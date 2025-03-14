using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Limit : MonoBehaviour
{

    // XとYの上限(仮）
    public float xLimitMax = 10.0f;
    public float xLimitMin = -10.0f;
    public float yLimitMax = 65.0f;
    public float yLimitMin = 0.0f;

    // Update is called once per frame
    void Update()
    {

        //　現在のポジションを保持する
        Vector3 currentPos = transform.position;

        // Mathf.ClampでX,Yの値それぞれが最小～最大の範囲内に収める。
        // 範囲を超えていたら範囲内の値を代入する
        currentPos.x = Mathf.Clamp(currentPos.x, xLimitMin, xLimitMax);
        currentPos.y = Mathf.Clamp(currentPos.y, yLimitMin, yLimitMax);

        //　positionをcurrentPosにする
        transform.position = currentPos;

    }
}