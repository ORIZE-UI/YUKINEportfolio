//履歴
//2024/05/02　151行Enemyタグ追加
//



using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using DG.Tweening;

public class SelfDestruction : MonoBehaviour
{
	[Tooltip("自爆までの時間を設定してください。(既定値：1.5秒)")]
	public float selfDestructionDelay = 1.5f;

	[Tooltip("自爆時のSphereColliderの半径の設定(既定値：1.5)")]
	public float newRadius = 1.5f;  // SphereColliderの自爆用のradiusの値

	[Tooltip("プレイヤーを消す時間の設定(既定値：2.0秒)")]
	public float playerDestroyTime = 2.0f;  // 自爆後にプレイヤーを消す時間

	[Tooltip("プレイヤーを消す間隔の設定(既定値：0.2秒)")]
	public float playerDestroyInterval = 0.2f;  //playerDestroyTimeから数値を引くための変数

	[Tooltip("自爆時に破壊するオブジェクトのタグの設定")]
	public List<string> destroyTags = new() { "Crash" };  // 自爆時に破壊するオブジェクトのタグ

	public GameObject effectPrefab; // エフェクトのプレハブ

	private bool wasExploded = false; // 自爆が行われたかどうか
	public bool WasExploded { get { return wasExploded; } }

	private float timePressed = 0.0f;
	private bool selfDestructionInitiated = false; // 自爆が始まったかどうか
	public bool IsSelfDestructed { get { return selfDestructionInitiated; } }

	public GameObject playerPrefab; // プレイヤーのプレハブ

	private float nowRadius = 0.0f;  // SphereColliderの元の当たり判定(退避用)
	private SphereCollider SphereCollider;  // プレイヤーの当たり判定

	private PlayerInput playerInput;

	private Vector3 cameraOriginalPosition; // カメラの初期位置

	[Tooltip("爆発の強さ")]
	public float explosionPower = 100.0f;

	public float respawnTime = 2.0f;
	private Vector3 startPos;  // 初期値リスポーン用



	public Animator animator;
	private GameObject player;
	Vector3 HolePosition;
	Vector3 playerPosition;
	Vector3 screenPosition;

	[Tooltip("穴が開くシェーダー＆開けたい穴の形が設定されたmaterialの指定")]
	public Material holeMaterial;
	[Tooltip("開ける穴のサイズ(既定値：0.1)")]
	public float HoleSize = 0.1f;//

	// Start is called before the first frame update
	void Start()
	{
		SphereCollider = GetComponent<SphereCollider>();  //現在のSphereColliderの値を取得
		nowRadius = SphereCollider.radius;
		playerInput = GetComponent<PlayerInput>();
		startPos = transform.position;
		wasExploded = false;

		// カメラの初期位置を取得
		cameraOriginalPosition = Camera.main.transform.position;
		// プレイヤーをシーンから検索して取得
		if (player == null)
		{
			player = GameObject.FindWithTag("Player"); // "Player" タグが付いたオブジェクトを探す
		}

		if (player == null)
		{
			Debug.LogError("Player オブジェクトが見つかりません。シーンに Player オブジェクトを配置してください。");
		}
		HoleReset();
	}

	// Update is called once per frame
	void Update()
	{
		// InputSystemを使ったボタンの状態をチェック
		var selfDestruction = playerInput.actions["SelfDestruction"].WasPerformedThisFrame();
		if (selfDestruction)
		{

			selfDestructionInitiated = true;

#if UNITY_EDITOR
			Debug.Log("自爆しました。");
#endif
		}

		// 自爆が開始されたら、時間をカウントし、自爆処理を行う
		if (selfDestructionInitiated == true)
		{
			animator.SetBool("ExplosionBool", true);

			timePressed += Time.deltaTime;
			Debug.Log("自爆までの時間：" + timePressed);
			if (timePressed >= selfDestructionDelay)
			{
				SelfDestruct();
				playerDestroyTime -= playerDestroyInterval;  //プレイヤーを消すカウント
#if UNITY_EDITOR
				Debug.LogError("test");
#endif
				if (playerDestroyTime <= 0.0f)
				{
					selfDestructionInitiated = false;   //自爆のboolをfalseに
					SphereCollider.radius = nowRadius;  //当たり判定の半径を元に戻す
					Destroy(gameObject);                //プレイヤーを破壊
					//GameManager.Instance.StartRespawnCoroutine(respawnTime, transform.position);
					GameManager.Instance.StartRespawnCoroutine(respawnTime, startPos);
				}
			}
		}
	}
	/// <summary>
	/// //背景に穴を開ける処理
	/// </summary>
	void OpenHole()
	{
		//playerPosition = player.transform.position;
		//screenPosition = Camera.main.WorldToViewportPoint(playerPosition);
		//holeMaterial.SetVector("_HoleCenter", new Vector4(screenPosition.x, screenPosition.y, 0, 0));
		holeMaterial.SetVector("_HoleCenter", new Vector4(0.5f, 0.5f, 0, 0));

		Debug.Log("穴を生成");
	}

	/// <summary>
	/// //背景の穴をリセット処理
	/// </summary>
	void HoleReset()
	{
		holeMaterial.SetVector("_HoleCenter", new Vector4(9999, 9999, 0, 0));
		Debug.Log("穴をリセット");
	}
	private void OnDestroy()
	{
		if (wasExploded)
		{
			// エフェクトのオブジェクトの生成
			Instantiate(effectPrefab, transform.position, Quaternion.identity);
		}
	}

	/// <summary>
	/// 自爆処理
	/// </summary>
	void SelfDestruct()
	{
		// ここに自爆処理を書く
#if UNITY_EDITOR
		Debug.Log("自爆しました。");
#endif
		// プレイヤーの位置を中心に一定範囲内を調べる
		float searchRadius = 3.0f;  // 検索する範囲の半径
		Collider[] colliders = Physics.OverlapSphere(transform.position, searchRadius);

		// フラグ：範囲内にDestroyObjが存在するかどうか
		bool destroyObjFound = false;

		// 範囲内のオブジェクトを調べる
		foreach (Collider col in colliders)
		{
			// オブジェクトの名前がDestroyObjならフラグをtrueにする
			if (col.gameObject.name == "DestroyObj")
			{
				destroyObjFound = true;
				break;  // 見つけたらループを終了
			}
		}

		// DestroyObjが範囲内に存在する場合のみ穴を開ける
		if (destroyObjFound)
		{
			OpenHole();  // 背景に穴を開ける処理を呼び出す
			Debug.Log("DestroyObjが範囲内に存在したため、穴を開けました。");
		}
		//	OpenHole();//穴を開ける

		SphereCollider.excludeLayers = 0;  //当たり判定を全てのレイヤーに対して行う
		SphereCollider.includeLayers = LayerMask.GetMask("Wall");  //当たり判定をWallレイヤーに対して行う

		SphereCollider.radius = newRadius;  //自爆用の当たり判定の半径の値に更新
											// Mathf.Lerp(現在の値, 目標の値, 時間)  //現在の値から目標の値に向かって時間をかけて変化させる
											//SphereCollider.radius = Mathf.Lerp(SphereCollider.radius, newRadius, 0.0f);  //自爆用の当たり判定の半径の値に更新  //自爆用の当たり判定の半径の値に更新


		Ray ray = new Ray(transform.position, Vector3.down);
		RaycastHit hit;
		if (Physics.Raycast(ray, out hit, 100.0f))
		{
			Collider[] cols = Physics.OverlapSphere(hit.point, newRadius);

			foreach (var col in cols)
			{

				if (col.GetComponent<Rigidbody>() != null)
				{
					col.GetComponent<Rigidbody>().AddExplosionForce(explosionPower, hit.point, newRadius);
				}
			}
		}

		wasExploded = true;  //自爆が行われた

		// 自爆時に揺れを発生させる
		StartShake(1.0f, 1.0f, 2, 10.0f, true);
	}

	// duration		時間
	// strength		揺れの強さ
	// vibrato		どのくらい振動するか
	// randomness	ランダム度合(0〜180)
	// fadeOut"		フェードアウトするか
	public void StartShake(float duration, float strength, int vibrato,
		float randomness, bool fadeOut)
	{
		// カメラの揺れを開始
		Camera.main.transform.DOShakePosition(duration, strength, vibrato, randomness, fadeOut)
			.OnComplete(() =>
			{
				// 揺れが終わった後にカメラの位置を元に戻す
				Camera.main.transform.position = cameraOriginalPosition;
			});
	}

	void OnCollisionEnter(Collision other)
	{
		// `destroyTags`に設定されたタグのオブジェクトと衝突したら破壊する
		foreach (var destryTag in destroyTags) // イテレーターを使ってdestroyTagsの中身を取り出す
		{
			// 衝突したオブジェクトのタグがdestroyTagsの中身と一致したら
			if (other.gameObject.CompareTag(destryTag))
			{
				if (selfDestructionInitiated == true)
				{
#if UNITY_EDITOR
					Debug.Log(destryTag + "を破壊しました。@" + System.Reflection.MethodBase.GetCurrentMethod().Name);
#endif
					//Destroy(other.gameObject);  // Crashタグのゲームオブジェクトを破壊
					//var orb = other.gameObject.GetComponent<Rigidbody>();
					//orb.AddExplosionForce
					//other.gameObject.SetActive(false);

				}
			}

		}


	}

	void OnCollisionStay(Collision other)  //こっちの方が早くDestroyが呼び出される
	{
		// `destroyTags`に設定されたタグのオブジェクトと衝突したら破壊する
		foreach (var destryTag in destroyTags) // イテレーターを使ってdestroyTagsの中身を取り出す
		{
			// 衝突したオブジェクトのタグがdestroyTagsの中身と一致したら
			if (other.gameObject.CompareTag(destryTag))
			{
				if (selfDestructionInitiated == true)
				{
#if UNITY_EDITOR
					Debug.Log(destryTag + "を破壊しました。@" + System.Reflection.MethodBase.GetCurrentMethod().Name);
#endif
					//Destroy(other.gameObject);  // Crashタグのゲームオブジェクトを破壊
					//other.gameObject.SetActive(false);
				}
			}

		}
	}

}
