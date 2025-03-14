using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VerticalDraft : MonoBehaviour
{
    public GameObject draftEffectPrefab;    // 上昇気流のエフェクトプレハブ
    public GameObject paperAirplane;        // 紙飛行機の参照
    public float draftForce = 10.0f;        // 上昇気流の影響力
    public float draftLifetime = 3.0f;      // 上昇気流の生存時間
    public float minDistanceZ = 5.0f;       // Z軸の最小生成距離
    public float rangeXY = 5.0f;            // X, Y軸のランダム生成範囲
    public float spawnInterval = 2.0f;      // 上昇気流の生成間隔
    public int maxDraftCount = 10;          // 同時に存在できる上昇気流の最大数
    public int maxCollisionCount = 3;       // 上昇気流に3回当たったら生成停止
    public float disableDuration = 5.0f;    // 生成停止期間（秒）

    private List<GameObject> activeDrafts = new List<GameObject>(); // 現在アクティブな上昇気流リスト
    private float lastSpawnTime = 0f;       // 最後に上昇気流を生成した時間
    private int collisionCount = 0;         // 紙飛行機が上昇気流に当たった回数
    private bool isDraftDisabled = false;   // 上昇気流生成停止中かどうかのフラグ

    public bool isStopped = false;          // ゴール時の生成停止フラグ
    void Update()
    {
        // ゴール到達で生成停止
        if (isStopped) return;

        // 上昇気流の生成は、生成間隔が経過し、上限数未満かつ生成が停止されていない場合のみ行う
        if (!isDraftDisabled && Time.time - lastSpawnTime > spawnInterval && activeDrafts.Count < maxDraftCount)
        {
            GenerateDrafts();
        }
    }

    void GenerateDrafts()
    {
        if (isStopped) return;  // ゴール後は生成をしない

        // 紙飛行機の位置を取得
        Vector3 airplanePosition = paperAirplane.transform.position;

        // ランダムなX, Y軸位置を決定 (プレイヤーから±rangeXY内)
        float randomX = Random.Range(airplanePosition.x - rangeXY, airplanePosition.x + rangeXY);
        float randomY = Random.Range(airplanePosition.y - rangeXY, airplanePosition.y + rangeXY);

        // Z軸はプレイヤーの前方にminDistanceZ以上離れて生成
        float randomZ = airplanePosition.z + Random.Range(minDistanceZ, minDistanceZ + 10f);

        // 上昇気流の位置を決定
        Vector3 draftPosition = new Vector3(randomX, randomY, randomZ);

        // 上昇気流のエフェクトを生成
        GameObject newDraft = Instantiate(draftEffectPrefab, draftPosition, Quaternion.identity);
        activeDrafts.Add(newDraft);  // リストに追加

        // 最後に生成した時間を更新
        lastSpawnTime = Time.time;

        // 一定時間後に上昇気流を削除
        StartCoroutine(RemoveDraftAfterTime(newDraft, draftLifetime));
    }

    void OnTriggerEnter(Collider other)
    {
        // 紙飛行機が上昇気流に当たった場合
        if (other.gameObject == paperAirplane)
        {
            // 紙飛行機が3回上昇気流に当たったら生成を停止
            collisionCount++;
            if (collisionCount >= maxCollisionCount)
            {
                StartCoroutine(DisableDraftsForSeconds(disableDuration));
                collisionCount = 0;  // カウントをリセット
            }
        }
    }

    void OnTriggerStay(Collider other)
    {
        // 紙飛行機が上昇気流にいる間は力を加え続けるが、加速を制限する
        if (other.gameObject == paperAirplane)
        {
            Rigidbody rb = paperAirplane.GetComponent<Rigidbody>();
            if (rb != null)
            {
                // 現在のY軸速度が一定以上にならないよう制限
                if (rb.velocity.y < 8f)  // 10fは速度の上限
                {
                    // 上昇力を加える
                    rb.AddForce(Vector3.up * draftForce, ForceMode.Acceleration);
                }
            }
        }
    }

    IEnumerator RemoveDraftAfterTime(GameObject draft, float time)
    {
        // 指定時間後にオブジェクトを削除
        yield return new WaitForSeconds(time);
        if (draft != null)
        {
            activeDrafts.Remove(draft);
            Destroy(draft);
        }
    }

    IEnumerator DisableDraftsForSeconds(float duration)
    {
        // 上昇気流の生成を停止
        isDraftDisabled = true;

        // 5秒待機
        yield return new WaitForSeconds(duration);

        // 上昇気流の生成を再開
        isDraftDisabled = false;
    }

    public void StopDrafts()
    {
        isStopped = true;  // ゴール時に生成停止
    }
}