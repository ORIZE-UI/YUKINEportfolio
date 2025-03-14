using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GenerateLevels : MonoBehaviour
{
    public GameObject[] level;
    public int zPos = 50;
    public bool creatingLevel = false;
    public int lvlNum;

    // Update is called once per frame
    void Update()
    {
        if (!creatingLevel)
        {
            creatingLevel = true;
            StartCoroutine(GenerateLvl());
        }
    }

    IEnumerator GenerateLvl()
    {
        lvlNum = Random.Range(0, 4); // 0, 1, 2, 3
        Instantiate(level[lvlNum], new Vector3(0, 0, zPos), Quaternion.identity);
        zPos += 50;
        yield return new WaitForSeconds(3);
        creatingLevel = false;
    }
}