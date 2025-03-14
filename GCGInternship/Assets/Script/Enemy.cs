using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    private Transform player;  // プレイヤーの参照
    public float destroyDistanceBehindPlayer = 5f;  // プレイヤーの後ろ何ユニットで削除するか

    void Start()
    {
        // プレイヤーをタグで検索して取得
        player = GameObject.FindGameObjectWithTag("Player").transform;
    }

    void Update()
    {
        //if (player == null) return;  // プレイヤーが見つからない場合は処理しない

        // エネミーがプレイヤーの後方10ユニット以上に移動した場合
        if (transform.position.z < player.position.z - destroyDistanceBehindPlayer)
        {
            Destroy(gameObject);  // エネミーを削除
        }
    }
}
