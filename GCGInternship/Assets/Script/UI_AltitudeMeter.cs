using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;  // UIの操作に必要

//===============================
//10/7 飛行高度に応じたメーター
//===============================

public class UI_AltitudeMeter : MonoBehaviour
{
    public GameObject paperAirplane;    // 紙飛行機の参照
    public RectTransform meterNeedle;   // メーターの針（カーソル）
    public float minAltitude = 0f;      // メーターの最小高度
    public float maxAltitude = 50f;     // メーターの最大高度
    public float minYPosition = -100f;  // メーター針の最小Y位置
    public float maxYPosition = 100f;   // メーター針の最大Y位置

    void Update()
    {
        // 紙飛行機のY軸位置（高度）を取得
        float airplaneY = paperAirplane.transform.position.y;

        // 高度をメーター範囲に正規化（minAltitudeからmaxAltitudeの間に収める）
        float normalizedAltitude = Mathf.InverseLerp(minAltitude, maxAltitude, airplaneY);

        // 正規化した高度を基に、メーター針のY位置を計算
        float needleYPosition = Mathf.Lerp(minYPosition, maxYPosition, normalizedAltitude);

        // 針（カーソル）のY位置を更新
        Vector2 needlePosition = meterNeedle.anchoredPosition;
        needlePosition.y = needleYPosition;
        meterNeedle.anchoredPosition = needlePosition;
    }
}
