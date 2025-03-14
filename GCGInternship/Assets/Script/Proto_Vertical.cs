using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Proto_Vertical : MonoBehaviour
{
    public GameObject verticalDraftPrefab;
    public float minSpawnDistanceZ = 10f;
    public float maxSpawnDistanceX = 5f;
    public float maxSpawnDistanceY = 5f;
    private GameObject player;

    public bool isStopped = false;          // ゴール時の生成停止フラグ
    void Start()
    {
        player = GameObject.FindWithTag("Player");

        // 初期位置に上昇気流を生成
        Vector3 specificPosition = new Vector3(-69, 72, 0);
        Instantiate(verticalDraftPrefab, specificPosition, Quaternion.identity);

        StartCoroutine(SpawnVerticalDrafts());
    }

    IEnumerator SpawnVerticalDrafts()
    {
        while (true)
        {
            if (isStopped) yield break;  // ゴール後は生成を停止

            float randomX = Random.Range(-maxSpawnDistanceX, maxSpawnDistanceX);
            float randomY = Random.Range(-maxSpawnDistanceY, maxSpawnDistanceY);

            Vector3 spawnPosition = player.transform.position + new Vector3(randomX, randomY, minSpawnDistanceZ);
            Instantiate(verticalDraftPrefab, spawnPosition, Quaternion.identity);

            yield return new WaitForSeconds(5f);
        }
    }

    // プレイヤーが上昇気流に触れた場合の処理
    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Player"))
        {
            PAP_Glide glideScript = other.GetComponent<PAP_Glide>();
            if (glideScript != null)
            {
                glideScript.StartAscending();
            }
        }
    }

    // プレイヤーが上昇気流から離れた場合の処理
    private void OnTriggerExit(Collider other)
    {
        if (other.CompareTag("Player"))
        {
            PAP_Glide glideScript = other.GetComponent<PAP_Glide>();
            if (glideScript != null)
            {
                glideScript.StopAscending();
            }
        }
    }

    public void StopDrafts()
    {
        isStopped = true;  // ゴール時に生成停止
    }
}
