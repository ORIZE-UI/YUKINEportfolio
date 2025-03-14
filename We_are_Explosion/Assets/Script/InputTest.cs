using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class InputTest : MonoBehaviour
{

	private PlayerInput playerInput;

    // Start is called before the first frame update
    void Start()
    {
        playerInput = GetComponent<PlayerInput>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

	public void OnHoldTest(InputAction.CallbackContext context)
	{
		Debug.Log("HoldTest");
		var Hold = playerInput.actions["test"].WasPerformedThisFrame();
		if (Hold)
		{
			Debug.Log("HoldTest: Performed");
		}
	}

	
}
