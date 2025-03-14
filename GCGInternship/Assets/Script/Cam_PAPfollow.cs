using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Cam_PAPfollow : MonoBehaviour
{
    public Transform target;                         // 追従するターゲット（紙飛行機）
    public Vector3 offset = new Vector3(0, 2, -5);   // カメラのオフセット
    public float followSpeed = 5.0f;                 // 追従速度

    // ゴール後のカメラ移動用（紙飛行機の位置に基づくオフセット）
    public Vector3 goalOffset = new Vector3(0, 4, -4);  // ゴール後のカメラ移動用オフセット（紙飛行機の相対位置）
    public float transitionSpeed = 2.0f;                // カメラがゴール位置に移動する速度
    public float targetXRotation = 22.82f;              // カメラのX軸回転角度

    private bool isFollowing = true;       // カメラが紙飛行機を追従しているかどうかのフラグ
    private bool isTransitioning = false;  // ゴール後に指定座標へ移動中かどうかのフラグ
    private Vector3 goalPosition;          // ゴール後のカメラ位置
    private Quaternion goalRotation;       // ゴール後のカメラ回転

    void LateUpdate()
    {
        if (isFollowing && target != null)
        {
            // ターゲットに追従
            Vector3 targetPosition = target.position + offset;
            transform.position = Vector3.Lerp(transform.position, targetPosition, followSpeed * Time.deltaTime);

            // ターゲット方向にカメラを向ける
            transform.LookAt(target);
        }
        else if (isTransitioning)
        {
            // ゴール後のカメラ移動処理
            SmoothTransitionToGoalPosition();
        }
    }

    // ゴール到達時にカメラの追従を解除し、指定座標に移動開始
    public void TransitionToGoalPosition()
    {
        if (target != null)
        {
            // ゴール後のカメラ位置を計算（紙飛行機の位置に基づいてY軸+4, Z軸-4）
            goalPosition = target.position + goalOffset;

            // ゴール後のカメラ回転を設定（Rotation.x = 22.82度、他の軸はターゲットに向けた回転を維持）
            Vector3 lookDirection = target.position - goalPosition;
            goalRotation = Quaternion.LookRotation(lookDirection);
            goalRotation = Quaternion.Euler(targetXRotation, goalRotation.eulerAngles.y, goalRotation.eulerAngles.z);
        }

        isFollowing = false;       // 追従を解除
        isTransitioning = true;    // ゴール後の位置への移動を開始
    }

    // カメラをゴール後の指定位置に移動させる処理
    private void SmoothTransitionToGoalPosition()
    {
        // カメラを滑らかにゴール後の指定位置に移動
        transform.position = Vector3.Lerp(transform.position, goalPosition, transitionSpeed * Time.deltaTime);

        // カメラの回転も滑らかに変更（Rotation.xを22.82度に設定）
        transform.rotation = Quaternion.Slerp(transform.rotation, goalRotation, transitionSpeed * Time.deltaTime);
    }
}