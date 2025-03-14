using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI_Operation : MonoBehaviour
{

    public void PauseGame()
    {
        Time.timeScale = 0;
    }

    public void ResumeGame()
    {
        Time.timeScale = 1;
    }
    // Start is called before the first frame update
    void Start()
    {
       // PauseGame();
    }

    //// Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.P))
        {
            PauseGame();
        }
        if (Input.GetKey(KeyCode.R))
        {
            ResumeGame();
        }

    }
}
