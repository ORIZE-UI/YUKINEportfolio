using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class ShakeKamera : MonoBehaviour
{
	// DOTweenを使用してカメラを揺らす
	private Tweener _shakeTweener;  // 揺らす用
	private Vector3 _initPosition;  // 初期位置

	// Start is called before the first frame update
	void Start()
	{
		// 初期位置を設定する
		_initPosition = transform.position;
	}

	/// 揺れ開始
	/// </summary>
	/// <param name="duration">時間</param>
	/// <param name="strength">揺れの強さ</param>
	/// <param name="vibrato">どのくらい振動するか</param>
	/// <param name="randomness">ランダム度合(0〜180)</param>
	/// <param name="fadeOut">フェードアウトするか</param>
	public void StartShake(float duration, float strength,
						   int vibrato, float randomness, bool fadeOut)
	{
		// 前回の処理が残っていれば停止して初期位置に戻す
		if (_shakeTweener != null)
		{
			_shakeTweener.Kill();
			gameObject.transform.position = _initPosition;
		}
		// 揺れ開始
		_shakeTweener = gameObject.transform.DOShakePosition(duration, strength, vibrato, randomness, fadeOut);
	}

	// Update is called once per frame
	void Update()
    {
		//// Uキーが押されたら揺れを開始する
		//if (Input.GetKeyDown(KeyCode.U))
		//{
		//	StartShake(1f, 3f, 10, 90f, true);
		//}
	}
}

