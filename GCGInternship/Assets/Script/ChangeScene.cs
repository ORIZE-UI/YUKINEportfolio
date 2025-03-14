using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class ChangeScene : MonoBehaviour
{
    // Update is called once per frame
    void Update()
    {
        // SpaceƒL[‚ª‰Ÿ‚³‚ê‚½‚ç
        if (Input.GetKeyDown(KeyCode.Return))
        {
            // PlayScene‚ÉØ‚è‘Ö‚¦
            SceneManager.LoadScene("GameStage");
        }
    }
    //public void LoadScene(string str)
    //{
    //    SceneManager.LoadScene(str);
    //}
}