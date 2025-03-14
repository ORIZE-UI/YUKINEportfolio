using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShutterController : MonoBehaviour
{
	[Tooltip("シャッターのオブジェクトを入れてください.")]
	public Transform Shutter; // シャッターのTransform
	[Tooltip("アニメーションの時間を設定してください.")]
	public float duration = 2f; // アニメーションの時間
	public List<string> switchTags = new () { "Switch" };
	public int paturn = 1;  //アニメーションのパターン
	private Vector3 originalPos;  // 初期座標
	private Vector3 originalScale; // 初期スケール
	private bool isOpen = false; // シャッターが開いているかどうかを示すフラグ

	void Start()
	{
		originalPos = Shutter.localPosition; // 初期座標を保存
		originalScale = Shutter.localScale; // 初期スケールを保存
	}

	void OnCollisionEnter(Collision other)
	{
		foreach (var switchTag in switchTags) // プレイヤーがスイッチのオブジェクトに接触したら
		{
			if(isOpen == false)
			{
				isOpen = true;
				if(paturn == 1)
				{
					StartCoroutine(AnimateTrans());
				}
				else if(paturn == 2)
				{
					StartCoroutine(AnimateScale());
				}
			}
		}
	}

	IEnumerator AnimateTrans()
	{
		float timer = 0f;
		while (timer < duration)
		{
			timer += Time.deltaTime;
			Shutter.Translate(Vector3.up * 0.5f * Time.deltaTime, Space.World); // 毎フレームごとに上昇させる
			yield return null;
		}
	}
	IEnumerator AnimateScale()
	{
		float timer = 0f;
		while (timer < duration)
		{
			timer += Time.deltaTime;
			// 時間経過に応じてシャッターのYスケールを変更する
			float newYScale = Mathf.Lerp(originalScale.y, 0f, timer / duration);
			Shutter.localScale = new Vector3(originalScale.x, newYScale, originalScale.z);
			Shutter.Translate(Vector3.up * 0.4f * Time.deltaTime, Space.World); // 毎フレームごとに上昇させる

			yield return null;
		}
	}
}

