using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BigEnemySpawner : MonoBehaviour
{
    public GameObject enemyPrefab;      // エネミーのプレハブ
    public string enemyPrefabName = "BigBird";  // プレハブ化された敵の名前（Resourcesフォルダ内にある）
    public GameObject Field;            //フィールドオブジェクトの参照
    public Transform player;            // プレイヤーの参照
    public float minSpawnInterval = 1f; // 最小の生成間隔
    public float maxSpawnInterval = 5f; // 最大の生成間隔
    public float minY = 5.0f;           // Y軸の最小生成距離
    public float maxY = 15.0f;          // Y軸の最大生成距離
    //public float heightAboveField = 5f; // フィールドの上に生成する高さ

    // 生成されるエネミーの数の範囲
    public int minSpawnCount = 1;       // 最小の生成数
    public int maxSpawnCount = 5;       // 最大の生成数

    private BoxCollider fieldCollider;  // フィールドの範囲を持つBoxCollider
    public bool isStopped = false;      // ゴール時の生成停止フラグ
    void Start()
    {
        // フィールドをタグで検索して取得
        Field = GameObject.FindGameObjectWithTag("TestField");


        // フィールドのBoxColliderを取得
        fieldCollider = Field.GetComponent<BoxCollider>();

        if (fieldCollider == null)
        {
            Debug.LogError("フィールドにBoxColliderがありません。");
            return;
        }

        // Resourcesフォルダからプレハブを読み込む
        if (enemyPrefab == null)
        {
            enemyPrefab = Resources.Load<GameObject>(enemyPrefabName);
        }

        // コルーチンを開始してエネミーのランダム生成を行う
        StartCoroutine(SpawnEnemyCoroutine());
    }


    IEnumerator SpawnEnemyCoroutine()
    {
        while (true)
        {
            if (isStopped) yield break;  // ゴール後は生成を停止

            // ランダムな生成間隔を取得
            float randomInterval = Random.Range(minSpawnInterval, maxSpawnInterval);

            // ランダムな間隔を待つ
            yield return new WaitForSeconds(randomInterval);

            // ランダムな生成数を決定
            int spawnCount = Random.Range(minSpawnCount, maxSpawnCount);

            // ランダムな数のエネミーを生成
            for (int i = 0; i < spawnCount; i++)
            {
                SpawnEnemy();
            }
        }
    }
    void SpawnEnemy()
    {
        if (isStopped) return;  // ゴール後は生成をしない

        // enemyPrefabがnullでないことを確認
        if (enemyPrefab != null)
        {
            // フィールド範囲の上にランダムな位置を取得
            Vector3 spawnPosition = GetRandomPositionAboveField();

            // 無効な位置(Vector3.zero)の場合は生成をスキップ
            if (spawnPosition == Vector3.zero)
            {
                return; // エネミーを生成しない
            }

            // エネミーを生成
            Instantiate(enemyPrefab, spawnPosition, Quaternion.identity);
        }
    }


    Vector3 GetRandomPositionAboveField()
    {
        // フィールドの中心とサイズを取得
        Vector3 center = fieldCollider.bounds.center;
        Vector3 size = fieldCollider.bounds.size;

        // Y軸はminYとmaxYの範囲でランダムに決定
        float randomY = Random.Range(minY, maxY);

        // xとzはフィールドの範囲内でランダムに決定
        float randomX = Random.Range(center.x - size.x / 2, center.x + size.x / 2);
        float randomZ = Random.Range(center.z - size.z / 2, center.z + size.z / 2);

        Vector3 randomPosition = new Vector3(randomX, randomY, randomZ);

        // プレイヤーの前方かどうかを確認
        Vector3 directionToEnemy = randomPosition - player.position;
        if (Vector3.Dot(player.forward, directionToEnemy) > 0)
        {
            // プレイヤーの前方ならその位置を返す
            return randomPosition;
        }
        else
        {
            // プレイヤーの後方ならnullを返して生成をスキップ
            return Vector3.zero; // 特定の無効値を返す
        }
    }

    public void StopSpawning()
    {
        isStopped = true;  // ゴール時に生成停止
    }
}