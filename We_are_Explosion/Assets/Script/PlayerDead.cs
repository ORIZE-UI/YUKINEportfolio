using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;


public class PlayerDeathOnBulletCollision : MonoBehaviour
{
	public GameObject playerPrefab; // プレイヤーのプレハブ
	private bool isInvincible = false; // 無敵状態を示すフラグ
	public float invincibilityDuration = 2.0f; // 無敵時間の長さ（秒）
	private float invincibilityTimer = 0.0f; // 無敵状態の経過時間を追跡するタイマー
	public GameObject newPlayer;
	public float respawnTime = 2.0f;
	private Vector3 startPos;
	void Start()
	{
		// リスポーン直後は無敵状態にする
		MakePlayerInvincible();
		startPos = transform.position;
	}

	private void Update()
	{
		if (isInvincible)
		{
			invincibilityTimer += Time.deltaTime;
			if (invincibilityTimer >= invincibilityDuration)
			{
				// 無敵時間が経過したら無敵状態を解除する
				isInvincible = false;
				invincibilityTimer = 0.0f;
				Debug.Log("無敵状態が解除されました。");
			}
		}
	}
	private void OnTriggerEnter(Collider other)
	{
		if (isInvincible)
		{
			// 無敵状態のときは衝突を無視する
			return;
		}
		if (other.CompareTag("Bullet")|| other.CompareTag("Light"))
		{
			Debug.Log("Player hit otherTag");
			Die();
		}
	}

	private void Die()
	{
		// 死亡時の処理をここに記述
		Destroy(gameObject);
		Debug.Log("Player Died");

		//GameManager.Instance.StartRespawnCoroutine(respawnTime, transform.position);
		GameManager.Instance.StartRespawnCoroutine(respawnTime, startPos);
	}

	//void RespawnPlayer()
	//{
	//	// プレイヤーをリスポーンさせる処理
	//	//Instantiate(playerPrefab, spawnPosition, Quaternion.identity);
	//	newPlayer = Instantiate(playerPrefab, transform.position, Quaternion.identity);
	//	// プレイヤーが生成された後に、必要なスクリプトを再度アタッチする
	//	SelfDestruction selfDestructionScript = newPlayer.GetComponent<SelfDestruction>();
	//	selfDestructionScript.enabled = true;
	//	PlayerMove playerMoveScropt = newPlayer.GetComponent<PlayerMove>();
	//	playerMoveScropt.enabled = true;
	//	//Grab GrabScript = newPlayer.GetComponent<Grab>();
	//	//GrabScript.enabled = true;
	//	PlayerDeathOnBulletCollision playerDethScript = newPlayer.GetComponent<PlayerDeathOnBulletCollision>();
	//	playerDethScript.enabled = true;
	//	PlayerInput playerInputScript = newPlayer.GetComponent<PlayerInput>();
	//	playerInputScript.enabled = true;
	//	newPlayer.SetActive(true);
	//}

	private void MakePlayerInvincible()
	{
		isInvincible = true;
		invincibilityTimer = 0.0f; // 無敵時間をリセット
		Debug.Log("無敵状態を開始しました。");
	}
}
