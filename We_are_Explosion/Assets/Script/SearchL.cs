using System.Collections;
using UnityEngine;

public class CubeController : MonoBehaviour
{
	[System.Serializable]
	public class ObjectData
	{
		public GameObject prefab;        // 生成するプレハブ
		public Vector3 positionOffset;   // スポーンポイントからの位置オフセット（インスペクターで設定）
		public Vector3 rotationOffset;   // スポーンポイントからの回転オフセット（インスペクターで設定）
	}

	public ObjectData[] objectsData;     // 各オブジェクトのデータ配列
	public Transform objectSpawnPoint;   // スポーンポイント（基準となる位置）
	public float spawnInterval = 2.0f;   // オブジェクトを表示している時間
	public float destroyInterval = 1.0f; // オブジェクトを消去するまでの時間

	private GameObject[] currentObjects; // 生成されたオブジェクトの配列

	void Start()
	{
		StartCoroutine(ToggleObjectsAndUI());
	}

	IEnumerator ToggleObjectsAndUI()
	{
		while (true)
		{
			// 全てのオブジェクトを同時に生成
			currentObjects = new GameObject[objectsData.Length];
			for (int i = 0; i < objectsData.Length; i++)
			{
				// スポーンポイントからの相対的な位置と回転を計算
				Vector3 spawnPosition = objectSpawnPoint.position + objectsData[i].positionOffset;
				Quaternion spawnRotation = Quaternion.Euler(objectSpawnPoint.rotation.eulerAngles + objectsData[i].rotationOffset);

				// オブジェクトを生成
				currentObjects[i] = Instantiate(objectsData[i].prefab, spawnPosition, spawnRotation);
				currentObjects[i].SetActive(true);
			}

			// 生成されたオブジェクトを指定時間保持
			yield return new WaitForSeconds(spawnInterval);

			// 生成された全てのオブジェクトを破棄
			for (int i = 0; i < currentObjects.Length; i++)
			{
				Destroy(currentObjects[i]);
			}

			// オブジェクト消去後の待機
			yield return new WaitForSeconds(destroyInterval);
		}
	}
}
