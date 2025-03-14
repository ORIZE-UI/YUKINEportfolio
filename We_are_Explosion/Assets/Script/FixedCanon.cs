using System.Collections;
using UnityEngine;

public class Turret3D : MonoBehaviour
{
	// インスペクターで設定可能なフィールド
	public GameObject bulletPrefab; // 発射する弾のプレハブ
	public Transform firePoint; // 弾の発射位置
	public float fireSpeed = 10f; // 弾の発射速度
	public float fireInterval = 1f; // 発射間隔
	public Vector3 fireDirection = Vector3.right; // 弾の発射方向 (X方向をインスペクターで設定可能)
	public bool fireNegativeX = false; // 弾を -X 方向に発射するかのフラグ
	public Collider detectionArea; // 当たり判定エリア (Trigger)
	public bool alwaysFire = false; // 常時発射モードの切り替え
	public Animator turretAnimator; // Animator コンポーネントの参照 (インスペクターで設定)

	private float fireTimer = 0f; // 発射間隔のタイマー
	private bool playerInRange = false; // プレイヤーが当たり判定内にいるかどうか

	void Update()
	{
		fireTimer += Time.deltaTime;

		// 弾を発射する条件
		if (alwaysFire || playerInRange)
		{
			if (fireTimer >= fireInterval)
			{
				FireBullet();
				fireTimer = 0f;

				// shotフラグをtrueに設定してアニメーションを再生
				turretAnimator.SetBool("shot", true);

				// shotフラグをリセットするコルーチンを開始（フラグを少し長めに保持する）
				StartCoroutine(ResetShotFlag());
			}
		}
	}

	// 弾を発射する処理
	void FireBullet()
	{
		// 発射方向を設定 (-X 方向に発射するかチェック)
		Vector3 adjustedFireDirection = fireNegativeX ? Vector3.left : fireDirection;

		// 弾を指定された位置に生成
		GameObject bullet = Instantiate(bulletPrefab, firePoint.position, firePoint.rotation);
		Rigidbody rb = bullet.GetComponent<Rigidbody>();

		// 発射方向と速度を適用（adjustedFireDirectionを正規化して、発射速度を掛け合わせる）
		rb.velocity = adjustedFireDirection.normalized * fireSpeed;
	}

	// shotフラグをリセットするコルーチン
	IEnumerator ResetShotFlag()
	{
		// 少し長めに待ってからshotフラグをfalseに戻す（アニメーションが完了する時間を想定）
		yield return new WaitForSeconds(0.5f); // 適切な時間（アニメーションが再生されるのに十分な長さ）
		turretAnimator.SetBool("shot", false);
	}

	// 当たり判定エリアにプレイヤーが入った場合
	void OnTriggerEnter(Collider other)
	{
		if (other.CompareTag("Player"))
		{
			playerInRange = true;
		}
	}

	// 当たり判定エリアからプレイヤーが出た場合
	void OnTriggerExit(Collider other)
	{
		if (other.CompareTag("Player"))
		{
			playerInRange = false;
		}
	}
}
