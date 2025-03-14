using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AutoGenerateTerrain : MonoBehaviour
{
    //public GameObject contextTerrain; 

    public float scale = 1.0f;
    public float freq = 0.01f;//周波数

    public float flatWidth = 0.1f;

    public float roadfreq = 0.005f;
    public float roadWidth = 0.05f;


    private float perlinHeight;

    private float road1Start;
    private float road1End;

    private float roafFlug;

    private float road2Start;
    private float road2End;


    [ContextMenu("生成")]
    private void makeGround()
    {
        TerrainData genTerrain = GetComponent<Terrain>().terrainData;

        var heights = new float[genTerrain.heightmapResolution, genTerrain.heightmapResolution];



        for (int x = 0; x < genTerrain.heightmapResolution; x++)
        {
            for (int y = 0; y < genTerrain.heightmapResolution; y++)
            {

                float r1height = Mathf.PerlinNoise(x * roadfreq, y * roadfreq) * 0.2f + 0.3f;
                float r2height = Mathf.PerlinNoise(x * roadfreq + 500, y * roadfreq + 500) * 0.2f + 0.5f;

                road1Start = r1height - flatWidth / 2;
                road1End = r1height + flatWidth / 2;

                road2Start = r2height - flatWidth / 2;
                road2End = r2height + flatWidth / 2;


                // Terrainの高さを算出

                // パーリンノイズから高さのベースを算出
                perlinHeight = Mathf.PerlinNoise(x * freq, y * freq);



                heights[x, y] = convSlope1(perlinHeight);

                roafFlug = Mathf.PerlinNoise(x * roadfreq, y * roadfreq);
            }
        }

        genTerrain.SetHeights(0, 0, heights);
    }

    private float convSlope1(float slope)
    {
        // 高い方の平地を作る。
        if (slope > road2Start)
        {

            //slope = slope - flatWidth;
            if (slope < road2End)
            {
                slope = road2Start;
            }
            else
            {
                slope -= flatWidth;
            }
        }

        if (slope > road1Start)
        {

            //slope = slope - flatWidth;
            if (slope < road1End)
            {
                slope = road1Start;
            }
            else
            {
                slope -= flatWidth;
            }
        }

        return slope;
    }

}