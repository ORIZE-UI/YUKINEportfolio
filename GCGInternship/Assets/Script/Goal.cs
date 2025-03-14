using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Goal : MonoBehaviour
{
    public GameObject chara;            //プレイヤー
    public Cam_PAPfollow cameraScript;  // カメラ追従スクリプト

    public VerticalDraft draftScript;   // 上昇気流のスクリプト参照
    public Proto_Vertical PdraScript;
    public EnemySpawner enemySpawner;   // 敵生成のスクリプト参照
    public BigEnemySpawner BenemySp;

    public GameObject paperAirplane;    // 紙飛行機の参照
    public float positionXTransitionSpeed = 1.0f; // X座標を滑らかに0にする速度
    //public float positionZTransitionSpeed = 1.0f;
    private bool isGoalReached = false; // ゴールフラグ

    public BGMController bgmController; // BGMControllerを参照
    private GameClearUIManager GCUI;       // GameOverUIManagerスクリプトの参照

    void Start()
    {
        // 上昇気流や敵のスクリプトを適切なオブジェクトから取得
        draftScript = FindObjectOfType<VerticalDraft>();  // シーン内のVerticalDraftを取得
        PdraScript = FindObjectOfType<Proto_Vertical>();  // シーン内のVerticalDraftを取得
        enemySpawner = FindObjectOfType<EnemySpawner>();  // シーン内のEnemySpawnerを取得
        BenemySp = FindObjectOfType<BigEnemySpawner>();   // シーン内のBigEnemySpawnerを取得
        bgmController = FindObjectOfType<BGMController>();
        GCUI = FindObjectOfType<GameClearUIManager>();
    }

    // ゴールに到達したときの処理
    public void OnTriggerEnter(Collider collision)
    {
        if (collision.name == chara.name)
        {
            chara.SetActive(true);

            isGoalReached = true;
            StartCoroutine(SmoothXPositionToZero());

            // 紙飛行機のZ座標を1990に固定
            Vector3 currentPosition = paperAirplane.transform.position;
            //paperAirplane.transform.position = new Vector3(currentPosition.x, currentPosition.y, 1895);

            // 上昇気流と敵の生成を停止
            if (draftScript != null)
            {
                draftScript.StopDrafts();
            }

            if(PdraScript!=null)
            {
                PdraScript.StopDrafts();
            }

            if (enemySpawner != null)
            {
                enemySpawner.StopSpawning();
            }

            if (BenemySp != null)
            {
                BenemySp.StopSpawning();
            }

            // GameClearUIManagerのTriggerGameClearを呼び出して、ゲームクリア画面の処理を開始
            if (GCUI != null)
            {
                GCUI.TriggerGameClear();
            }

            // ゴール時にBGMのフェードアウトを開始
            if (bgmController != null)
            {
                bgmController.StartFadeOut();
            }
        }
    }

    // 紙飛行機のX座標を滑らかに0にする処理
    private IEnumerator SmoothXPositionToZero()
    {
        while (isGoalReached)
        {
            Vector3 currentPosition = paperAirplane.transform.position;

            // X座標を滑らかに0にする
            float newX = Mathf.Lerp(currentPosition.x, 0, Time.deltaTime * positionXTransitionSpeed);
            //paperAirplane.transform.position = new Vector3(newX, currentPosition.y, 1895);

            yield return null;
        }
    }
}