using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BGMVolumeController : MonoBehaviour
{
    public Transform paperAirplane;           // 紙飛行機のTransform
    public BGMController bgmController;       // BGMControllerを参照
    public float maxAltitude = 100f;          // 最大高度（この高さで最大音量）
    public float minAltitude = 0f;            // 最小高度（この高さで最小音量）
    public float altitudeVolumeFactor = 0.5f; // 高度補正用の倍率

    void Update()
    {
        if (paperAirplane != null && bgmController != null)
        {
            // 高度に応じた音量調整
            float altitude = Mathf.Clamp(paperAirplane.position.y, minAltitude, maxAltitude);
            float heightFactor = Mathf.InverseLerp(minAltitude, maxAltitude, altitude);
            float volumeAdjustment = Mathf.Lerp(0.1f, 1.0f, heightFactor) * altitudeVolumeFactor;
            bgmController.bgmAudioSource.volume = bgmController.baseVolume * volumeAdjustment;
        }
    }
}
