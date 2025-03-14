using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;

public class FloatingEnemy : MonoBehaviour
{
	private float nTime = 0;		// カウント用
	private bool isLR = false;		// 左向きならfalseで作ること
	private Rigidbody rb;           // 浮いてる敵用Rigidbody
	[Tooltip("警戒解除時間(秒じゃなく更新頻度)")]
	[SerializeField] private int delay;
	[Tooltip("動く速さ")]
	[SerializeField] private float moveSpeed;
	[Tooltip("最大移動距離")]
	[SerializeField] private float moveDistance;
	[Tooltip("振り向く速さ")]
	[SerializeField] private float RotationSpeed; 
	[Tooltip("索敵範囲")]
	[SerializeField] private float RotationDistance;
	private Vector3 startPos;                           // 初期位置
	private Quaternion startRotation;					// 初期向き
	private Vector3 moveDirection = Vector3.right;      // 移動方向(右)
	private Transform TPlayer;							// プレイヤーの位置、角度、サイズ情報取得

	// Start is called before the first frame update
	void Start()
    {
		// 浮いてる敵の初期情報取得
		startPos = transform.position;
		startRotation = transform.rotation;
		// 浮いてる敵のRigidbody取得
		rb = GetComponent<Rigidbody>();
		// プレイヤーのタグから情報取得
		TPlayer = GameObject.FindGameObjectWithTag("Player").transform;
    }

    // Update is called once per frame
    void Update()
    {
		nTime += 1; // 1 = 1秒になるよう計算
		// プレイヤーが検出範囲内にいるかどうかをチェック
		if (Vector3.Distance(transform.position, TPlayer.position) <= RotationDistance)
		{
			Debug.Log("敵発見");
			nTime = 0;
			// プレイヤーの方向を計算
			Vector3 direction = TPlayer.position - transform.position;
			float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;
			Quaternion targetRotation = Quaternion.Euler(new Vector3(0, 0, angle));
			// スムーズに回転する
			transform.rotation = Quaternion.Slerp(transform.rotation, targetRotation, RotationSpeed * Time.deltaTime);
		}
		if (nTime >= delay)
		{
			Debug.Log("敵逃走");
			// 時間がたったら最初の向きに戻る処理
			// 回転を緩やかにする
			transform.rotation = Quaternion.Slerp(transform.rotation, startRotation, RotationSpeed * Time.deltaTime);
		}
		// 指定した距離だけ移動したら、移動方向を反転させる
		if (Vector3.Distance(startPos, transform.position) >= moveDistance)
		{
			moveDirection = -moveDirection;	// 最大距離に達したら移動方向(ベクトル)を反転させる
		}

		// 移動方向に速度を掛けて移動させる
		rb.velocity = moveDirection * moveSpeed;
	}
}
