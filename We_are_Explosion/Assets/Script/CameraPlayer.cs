using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraPlayer : MonoBehaviour
{
	[Tooltip("Playerのオブジェクト")]
	public GameObject playerObj;

	private Vector3 offset;

	void Start()
	{
		// タグが`Player`のオブジェクトを取得
		playerObj = GameObject.FindGameObjectWithTag("Player");
		offset = transform.position - playerObj.transform.position;
	}

	// Update関数の後に実行される
	void LateUpdate()
	{
		if (playerObj != null)
		{
			transform.position = playerObj.transform.position + offset;
		}
		else
		{
			// プレイヤーオブジェクトがnullになった場合の対処
			playerObj = GameObject.FindGameObjectWithTag("Player");
			if (playerObj != null)
			{
				offset = transform.position - playerObj.transform.position;
			}
		}
	}

}
