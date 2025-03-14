using System.Collections;
using UnityEngine;

public class AutoMoveAndReflect : MonoBehaviour
{
	public float speed = 1f; // x軸の移動速度
	public string targetTag = "Crash"; // 反転を行うタグ
	public string targetTag1 = "Ground"; // 反転を行うタグ
	public string playerTag = "Player"; // プレイヤーのタグ
	public Transform patrolRayPoint; // Patrol用Rayの起点
	public float patrolRayDistance = 0.5f; // Patrol用Rayの長さ
	public Transform radarColliderTransform; // レーダー用コライダーのTransform
	public Vector3 radarColliderSize = new Vector3(2f, 1f, 5f); // レーダー用BoxColliderのサイズ
	public GameObject bulletPrefab; // 弾のプレハブ
	public float bulletSpeed = 20f; // 弾の速度
	public float fireRate = 0.5f; // 発射間隔

	[Tooltip("爆発に巻き込まれてから消えるまでの時間")]
	public float disapperTime = 1.0f;
	[Tooltip("(上限速度) 爆発によって飛ばされたときの速度によって消す")]
	public float velocityMagnitude = 10.0f;

	private Vector3 direction = Vector3.right; // 初期の移動方向
	private Vector3 patrolRayDirection = new Vector3(-1, -1, 0); // Patrol用Rayの初期方向
	private bool isChasingPlayer = false;
	private bool isWaiting = false;
	private bool isGrounded = true; // 足場があるかどうかを示すフラグ
	private float waitTime = 2f;
	private float lostPlayerTime = 3f;
	private bool isFiring = false; // 弾発射中かどうかのフラグ
	private Quaternion originalRotation; // 初期の回転を保持
	private float nextFireTime = 0f;

	public Animator animator;

	public LayerMask detectionLayerMask; // 検知したいレイヤーをインスペクターで設定


	//[SerializeField] private GameObject destroyObj = null;
	private SelfDestruction selfDestruction; // 自爆スクリプト

	void Start()
	{
		// レーダー用コライダーの設定
		BoxCollider radarCollider = radarColliderTransform.gameObject.AddComponent<BoxCollider>();
		radarCollider.isTrigger = true;
		radarCollider.size = radarColliderSize;
		radarCollider.center = new Vector3(-radarColliderSize.x / 2, 0, 0); // レーダーを左側に配置
		originalRotation = transform.rotation; // 初期の回転を保存

		// 自爆スクリプトの取得
		selfDestruction = GameObject.FindWithTag("Player").GetComponent<SelfDestruction>();
	}

	void Update()
	{
		if (isWaiting)
		{
			return;
		}
		if (selfDestruction == null)
		{
			selfDestruction = GameObject.FindWithTag("Player").GetComponent<SelfDestruction>();
			return;
		}

		if (isChasingPlayer)
		{
			// プレイヤー追跡中の処理
			if (CheckForGround())
			{
				isGrounded = true;
				ChasePlayer();
			}
			else
			{
				isGrounded = false;
			}
			if (Time.time > nextFireTime)
			{

				nextFireTime = Time.time + fireRate;
#if UNITY_EDITOR
				Debug.Log("弾発射");
#endif
				FireBullet();
				isFiring = true; // 発射中フラグを設定
				animator.SetBool("IsWalk", false);
				animator.SetBool("IsFire", true);
			}
		}
		else
		{

			// x軸に沿ってオブジェクトを移動
			transform.Translate(speed * Time.deltaTime * direction);
			Patrol();
			Debug.Log("哨戒");
			animator.SetBool("IsWalk", true);
			animator.SetBool("IsFire", false);

		}

		if (selfDestruction != null && selfDestruction.WasExploded == true)
		{
			Rigidbody rb = GetComponent<Rigidbody>();
			if (IsWithSelfDestructionRange() || rb.velocity.magnitude > velocityMagnitude)
			{
				Destroy(gameObject, disapperTime);
				//StartCoroutine(DestroyAfterDelay(disapperTime));
			}
		}

		
	}

	public bool IsWithSelfDestructionRange()
	{
		// 自爆スクリプトがない場合はfalseを返す
		if (selfDestruction == null)
		{
			return false;
		}
		var player = GameObject.FindWithTag("Player");
		player.transform.position = selfDestruction.transform.position;
		player.GetComponent<SphereCollider>().radius = selfDestruction.GetComponent<SphereCollider>().radius;
		player.GetComponent<SphereCollider>().center = selfDestruction.GetComponent<SphereCollider>().center;

		float distance = Vector3.Distance(transform.position, selfDestruction.transform.position);

		if (distance <= selfDestruction.GetComponent<SphereCollider>().radius /** selfDestruction.transform.localScale.x*/)
		{
			return true;
		}
		else
		{
			return false;
		}


	}

	private void Patrol()
	{
		// Raycastを使って斜め左下にRayを発射(往復移動用)
		Ray ray = new(patrolRayPoint.position, patrolRayDirection);
		RaycastHit hit;

		// Rayを可視化
		Debug.DrawRay(patrolRayPoint.position, patrolRayDirection * patrolRayDistance, Color.red);

		// Rayが何にも当たらなかった場合、または当たったがタグが指定したものでない場合、移動方向とy軸を反転
		if (Physics.Raycast(ray, out hit, patrolRayDistance, detectionLayerMask))
		{
			if (!hit.collider.CompareTag(targetTag1))
			{
				Reflect();
			}
		}
		else
		{
			Reflect();
		}

	}

	private void OnCollisionEnter(Collision collision)
	{
		// 他のオブジェクトと衝突したとき、そのオブジェクトが指定したタグを持っている場合
		if (collision.collider.CompareTag(targetTag))
		{
			Reflect();
		}
	}

	private void OnTriggerEnter(Collider other)
	{
		if (other.CompareTag(playerTag))
		{
			// プレイヤーとの間に障害物がないか確認
			RaycastHit hit;
			Vector3 directionToPlayer = (other.transform.position - transform.position).normalized;

			if (Physics.Raycast(transform.position, directionToPlayer, out hit, Mathf.Infinity, detectionLayerMask))
			{
				if (hit.collider.CompareTag(playerTag))
				{
#if UNITY_EDITOR
					Debug.Log("プレイヤーを発見");
#endif
					StartChasingPlayer();
				}
			}
		}







//		// プレイヤーを検知したとき
//		if (other.CompareTag(playerTag))
//		{
//#if UNITY_EDITOR
//			Debug.Log("プレイヤーを発見");
//#endif
//			StartChasingPlayer();
//		}
	}

	private void OnTriggerExit(Collider other)
	{
		// プレイヤーが離れたとき
		if (other.CompareTag(playerTag))
		{
#if UNITY_EDITOR
			Debug.Log("プレイヤーをロスト");
#endif
			StartCoroutine(LostPlayerCoroutine());
		}
	}

	private void Reflect()
	{
#if UNITY_EDITOR
		Debug.Log("敵が反転");
#endif
		// オブジェクトのy軸を180度回転
		transform.Rotate(0f, 180f, 0f);

		// Patrol用Rayの方向のy成分を反転
		patrolRayDirection.x = -patrolRayDirection.x;
	}

	private void StartChasingPlayer()
	{
		isChasingPlayer = true;
		StopAllCoroutines(); // コルーチンをすべて停止
	}

	private void ChasePlayer()
	{
		if (!isFiring && isGrounded)
		{
			transform.Translate(direction * speed * Time.deltaTime);
		}
	}

	private bool CheckForGround()
	{
		Ray ray = new(patrolRayPoint.position, patrolRayDirection);
		RaycastHit hit;
		return Physics.Raycast(ray, out hit, patrolRayDistance) && hit.collider.CompareTag(targetTag1);
	}

	private IEnumerator LostPlayerCoroutine()
	{
		isChasingPlayer = false; // プレイヤーを追跡中でないことを設定
		yield return new WaitForSeconds(lostPlayerTime); // プレイヤーを見失った後の待機時間
		isWaiting = true; // 待機中であることを設定
		yield return new WaitForSeconds(waitTime); // 待機時間
		isWaiting = false; // 待機終了
	}

	private void FireBullet()
	{
		GameObject bullet = Instantiate(bulletPrefab, transform.position, transform.rotation);
		Rigidbody rb = bullet.GetComponent<Rigidbody>();
		if (rb != null)
		{

			rb.velocity = transform.right * -bulletSpeed;
		}
		StartCoroutine(ResetFiringFlag());
	}
	private IEnumerator ResetFiringFlag()
	{
		yield return new WaitForSeconds(1.0f); // 適切な待機時間を設定
		isFiring = false;
	}

	private IEnumerator DestroyAfterDelay(float Delay)
	{
		yield return new WaitForSeconds(Delay);
		Destroy(gameObject);
	}

	
}
