using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Stage_VD : MonoBehaviour
{
    // 上昇値（Inspectorで変更可能）
    [SerializeField]
    private float upwardForce = 5.0f;

    // 上昇気流が削除されるプレイヤーからの距離
    [SerializeField]
    private float deleteDistance = 20.0f;

    // 紙飛行機（プレイヤー）の参照
    private GameObject paperAirplane;

    void Start()
    {
        // プレイヤーのオブジェクトを取得
        paperAirplane = GameObject.FindWithTag("Player");
    }

    void Update()
    {
        // プレイヤーが上昇気流を通過した後、削除する
        if (paperAirplane != null && transform.position.z < paperAirplane.transform.position.z - deleteDistance)
        {
            Destroy(gameObject); // 上昇気流オブジェクトの削除
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        // 紙飛行機が上昇気流に触れた時、上昇させる
        if (other.gameObject == paperAirplane)
        {
            Rigidbody rb = other.GetComponent<Rigidbody>();
            if (rb != null)
            {
                // 上昇力を与える
                rb.AddForce(Vector3.up * upwardForce, ForceMode.VelocityChange);
            }
        }
    }
}