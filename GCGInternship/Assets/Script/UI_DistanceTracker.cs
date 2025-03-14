using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;  // TextMeshProを使用するために必要

//===============================
//10/7 飛行距離に応じたメーター
//===============================

public class UI_DistanceTracker : MonoBehaviour
{
    public GameObject paperAirplane;      // 紙飛行機の参照
    public TextMeshProUGUI distanceText;  // UIに表示するTextMeshProのテキスト
    private Vector3 initialPosition;      // 初期位置を保存する変数

    void Start()
    {
        // 紙飛行機の初期位置を取得
        initialPosition = paperAirplane.transform.position;
    }

    void Update()
    {
        // Z軸の進行距離を計算
        float distanceTraveled = paperAirplane.transform.position.z - initialPosition.z;

        // 距離を整数にしてTextMeshProに表示
        distanceText.text = distanceTraveled.ToString("00000");
    }
}
