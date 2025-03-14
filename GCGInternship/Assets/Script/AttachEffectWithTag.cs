using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AttachEffectWithTag : MonoBehaviour
{
    public GameObject wingEffectPrefab;  // 翼にアタッチするエフェクトのPrefab

    void Start()
    {
        // "Wing"タグを持つ全てのオブジェクトを取得
        GameObject[] wings = GameObject.FindGameObjectsWithTag("Wing");

        // 各翼オブジェクトにエフェクトをアタッチ
        foreach (GameObject wing in wings)
        {
            GameObject effect = Instantiate(wingEffectPrefab, wing.transform);
            effect.transform.localPosition = Vector3.zero;  // 翼の位置にエフェクトを合わせる
        }
    }
}
