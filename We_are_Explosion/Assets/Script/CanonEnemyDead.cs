using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CanonEnemyDead : MonoBehaviour
{
	private void OnTriggerEnter(Collider other)
	{
		if (other.CompareTag("PlayerBullet"))
		{
			Debug.Log("hit bullet enemy");
			Die();
		}
	}

	private void Die()
	{
		// 死亡時の処理をここに記述
		Destroy(gameObject);
		Debug.Log("enemyDied");
	}
}




