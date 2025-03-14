using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Timer : MonoBehaviour
{

	void OnEnable()
	{
		// シーンが有効になるときにタイマーを開始
		GameManager.Instance.StartTimer();
	}

	void OnDisable()
	{
		// シーンが無効になるときにタイマーを停止
		GameManager.Instance.StopTimer();
	}
}
