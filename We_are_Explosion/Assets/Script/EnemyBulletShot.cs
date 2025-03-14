////履歴
////2024/05/02 このスクリプトを作成
/////2024/05/04敵オブジェクトの回転方向に対して弾の発射対応

using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Security.Cryptography;
using Unity.VisualScripting;
using UnityEngine;

public class EnemyShooter : MonoBehaviour
{
	public GameObject EnemyBulletPrefab;    // 弾敵オブジェクトのプレハブ
	public Transform ShootPoint;            // 弾の発射位置
	public float ShootInterval = 0.5f;      // 弾の発射間隔
	public float BulletLifetime = 3.0f;     // 弾の生存時間
	private float timer = 0.0f;				// タイマー
	private Transform TPlayer;              // プレイヤーの位置、角度、サイズ情報取得
	[Tooltip("弾の移動速度")]
	[SerializeField] private float moveSpeed = 5.0f;
	[Tooltip("追従速度(振り向き速度)")]
	[SerializeField] private float trackingSpeed = 2.0f;
	[Tooltip("追跡速度")]
	[SerializeField] private float homingSpeed = 2.0f;
	[Tooltip("索敵範囲")]
	[SerializeField] private float distance = 5.0f;
	
	// 使う弾の種類
	[Tooltip("同じ方向に打ち続ける弾(常)")]
	[SerializeField] private bool isFullAutoBullet = false;
	[Tooltip("同じ方向に打ち続ける弾(検知)")]
	[SerializeField] private bool isSearchFullAutoBullet = false;
	[Tooltip("追従弾(常)")]
	[SerializeField] private bool isTrackingBullet = false;
	[Tooltip("追従弾(検知)")]
	[SerializeField] private bool isSearchTrackingBullet = false;
	[Tooltip("ホーミング弾(常)")]
	[SerializeField] private bool isHomingBullet = false;

	// 打ち続ける弾の方向
	private Vector3 direcsion;	// 方向
	[Tooltip("オブジェクトから見て：上(打ち続ける弾の方向)")]
	[SerializeField] private bool Up = false;
	[Tooltip("オブジェクトから見て：下(打ち続ける弾の方向)")]
	[SerializeField] private bool Down = false;
	[Tooltip("オブジェクトから見て：前(打ち続ける弾の方向)")]
	[SerializeField] private bool forward = false;
	[Tooltip("オブジェクトから見て：後(打ち続ける弾の方向)")]
	[SerializeField] private bool back = false;

	private void Start()
	{
		// プレイヤーのタグから情報取得
		TPlayer = GameObject.FindGameObjectWithTag("Player").transform;
	}

	void Update()
	{
		// タイマーを更新
		timer += Time.deltaTime;

		if (Up)
		{
			direcsion = transform.up;
		}
		else if (Down)
		{
			direcsion = -transform.up;
		}
		else if (forward)
		{
			direcsion = -transform.right;
		}
		else if (back)
		{
			direcsion = transform.right;
		}

		if (timer >= ShootInterval)
		{
			// 弾の種類によって入る関数を変える
			if (isFullAutoBullet == true)
			{
				FullAutoBullet();
			}
			if (isSearchFullAutoBullet == true)
			{
				SearchFullAutoBullet();
			}
			if (isTrackingBullet == true)
			{
				TrackingBullet();
			}
			if (isSearchTrackingBullet == true)
			{
				SearchTrackingBullet();
			}
			if (isHomingBullet == true)
			{
				HomingBullet();
			}
			timer = 0.0f;
		}
	}

	void FullAutoBullet()
	{
		// 弾オブジェクトをプレハブから生成
		GameObject bullet = Instantiate(EnemyBulletPrefab, ShootPoint.position, Quaternion.identity);

		// 弾敵オブジェクトの移動方向を設定
		StartCoroutine(MoveEnemy(bullet.transform));
		StartCoroutine(DestroyBulletAfterTime(bullet, BulletLifetime));
	}

	void SearchFullAutoBullet()
	{
		if (Vector3.Distance(transform.position, TPlayer.position) <= distance)
		{
			// 弾オブジェクトをプレハブから生成
			GameObject bullet = Instantiate(EnemyBulletPrefab, ShootPoint.position, Quaternion.identity);

			// 弾敵オブジェクトの移動方向を設定
			StartCoroutine(MoveEnemy(bullet.transform));
			StartCoroutine(DestroyBulletAfterTime(bullet, BulletLifetime));
		}
	}

	void TrackingBullet()
	{	
		// プレイヤーの方向を計算
		Vector3 direction = TPlayer.position - transform.position;
		float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;
		Quaternion targetRotation = Quaternion.Euler(new Vector3(0, 0, angle));
		// スムーズに回転する
		transform.rotation = Quaternion.Slerp(transform.rotation, targetRotation, trackingSpeed * Time.deltaTime);

		// 弾オブジェクトをプレハブから生成
		GameObject bullet = Instantiate(EnemyBulletPrefab, ShootPoint.position, Quaternion.identity);

		// 弾敵オブジェクトの移動方向を設定
		StartCoroutine(MoveEnemy(bullet.transform));
		StartCoroutine(DestroyBulletAfterTime(bullet, BulletLifetime));
	}

	void SearchTrackingBullet()
	{
		if (Vector3.Distance(transform.position, TPlayer.position) <= distance)
		{
			
		}
	}

	void HomingBullet()
	{

	}

	// 弾敵オブジェクトを移動させるルーチン
	IEnumerator MoveEnemy(Transform enemyTransform)
	{
		while (true)
		{
			if (enemyTransform != null)
			{
				// 弾敵オブジェクトを移動方向に移動させる
				enemyTransform.Translate(direcsion * moveSpeed * Time.deltaTime);
			}
			yield return null;
		}
	}

	// 指定した時間後に弾を破棄する
	IEnumerator DestroyBulletAfterTime(GameObject bullet, float lifetime)
	{
		yield return new WaitForSeconds(lifetime);
		if (bullet != null)
		{
			Destroy(bullet);
		}
	}
}
