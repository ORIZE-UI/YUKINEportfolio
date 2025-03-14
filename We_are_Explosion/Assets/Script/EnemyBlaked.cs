using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyBlaked : MonoBehaviour
{
	public float blowPower = 500.0f; // 吹き飛ばされる力
	public float minAngle = 0.0f; // 吹き飛ばされる最小角度
	public float maxAngle = 360.0f; // 吹き飛ばされる最大角度

	// Unity上でのみコンパイルする
#if UNITY_EDITOR
	[Tooltip("テストモードかどうか")]
	public bool isTest = false; // テストモードかどうか
#endif

	// Start is called before the first frame update
	void Start()
	{
#if UNITY_EDITOR
		if (isTest == true)
		{
			Vector3 test = new(transform.position.x, transform.position.y, -0.5f);
			ExplodeChild(test);
		}
#endif
	}

	// Update is called once per frame
	void Update()
	{
		
	}

	// 吹き飛ばす処理を行う
	public void Explode(Vector3 hitPos)
	{
		// Rigidbodyの取得/なければ追加
		Rigidbody rb = GetComponent<Rigidbody>();
		if (rb == null)
		{
			rb = gameObject.AddComponent<Rigidbody>();
		}

		// 吹き飛ばす方向を求める
		Vector3 direction = (transform.position - hitPos).normalized;

		// 吹き飛ばす
		rb.AddForce(direction * blowPower);
	}

	// 子オブジェクトを吹き飛ばす処理を行う
	public void ExplodeChild(Vector3 hitPos)
	{
		// 子オブジェクトを取得
		Transform[] children = GetComponentsInChildren<Transform>();

		foreach (Transform child in children)
		{
			// Rigidbodyの取得/なければ追加
			Rigidbody rb = child.GetComponent<Rigidbody>();
			if (rb == null)
			{
				rb = child.gameObject.AddComponent<Rigidbody>();
			}

			// 吹き飛ばす方向を求める
			Vector3 direction = (child.position - hitPos).normalized;

			// 吹き飛ばす
			rb.AddForce(direction * blowPower);
		}
	}
}
