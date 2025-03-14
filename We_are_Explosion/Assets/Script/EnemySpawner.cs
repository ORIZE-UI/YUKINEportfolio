using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySpawner : MonoBehaviour
{
	[Tooltip("敵のPrefab")]
	public GameObject enemyPrefab;

	[Tooltip("敵がスポーンする間隔(秒)")]
	public float spawnInterval = 12.0f;

	[Tooltip("出現位置")]
	public Transform spawnPoint;

	[Tooltip("プレイヤーがこの距離以内にいたらスポーンしない")]
	public float notSpawnRange = 1.0f;

	private GameObject player;
	

	private float timeSinceLastSpawn = 0.0f;

	// Start is called before the first frame update
	void Start()
	{
		player = GameObject.FindWithTag("Player");
		
	}

	// Update is called once per frame
	void Update()
	{
		if (player == null)
		{
			player = GameObject.FindWithTag("Player");
		}

		if (Vector3.Distance(player.transform.position, spawnPoint.position) > notSpawnRange)
		{
			// 経過時間をカウント
			timeSinceLastSpawn += Time.deltaTime;

			if (timeSinceLastSpawn >= spawnInterval)
			{
				// 敵をスポーンさせる
				SpawnEnemy();

				// カウントをリセット
				timeSinceLastSpawn = 0.0f;

			}
		}
	}

	void SpawnEnemy()
	{
		// 敵をスポーン位置に生成
		Instantiate(enemyPrefab, spawnPoint.position, spawnPoint.rotation);
	}
}
