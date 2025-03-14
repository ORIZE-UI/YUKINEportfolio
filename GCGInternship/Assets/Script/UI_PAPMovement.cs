using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_PAPMovement : MonoBehaviour
{
    public Transform paperAirplane;        // 紙飛行機のTransform
    public RectTransform uiPaperAirplane;  // UIの紙飛行機
    public RectTransform uiStart;          // スタート地点のUI
    public RectTransform uiGoal;           // ゴール地点のUI

    public Transform stageStart;           // ステージ上のスタート地点（実際のステージのTransform）
    public Transform stageGoal;            // ステージ上のゴール地点（実際のステージのTransform）

    private float totalDistance;           // ステージ上のスタートからゴールまでの距離

    void Start()
    {
        // ステージ上のスタート地点とゴール地点の距離を計算
        totalDistance = Vector3.Distance(stageStart.position, stageGoal.position);
    }

    void Update()
    {
        // 紙飛行機の進行度（ステージ上での進行距離）を計算
        float distanceTraveled = Vector3.Distance(stageStart.position, paperAirplane.position);
        float progress = distanceTraveled / totalDistance; // 0～1の進行度

        // UIの紙飛行機をスタートからゴールまで動かす
        uiPaperAirplane.position = Vector3.Lerp(uiStart.position, uiGoal.position, progress);
    }
}