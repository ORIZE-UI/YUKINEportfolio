using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
	public float lifeTime = 50f; // 弾が消えるまでの時間

	void Start()
	{
		// lifeTime後に弾を破壊する
		Destroy(gameObject, lifeTime);
	}

	void OnCollisionEnter(Collision collision)
	{
		// 衝突したオブジェクトが指定したタグを持っている場合、弾を破壊する
		if ((collision.collider.CompareTag("Crash")) || (collision.collider.CompareTag("Ground")))
		{
			Destroy(gameObject);
		}
	}
}
