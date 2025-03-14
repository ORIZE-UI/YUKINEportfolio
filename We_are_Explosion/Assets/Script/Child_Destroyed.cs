using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Child_Destroyed : MonoBehaviour
{
	// 子オブジェクトのRigidbodyを取得
	private Transform parent;

	private Rigidbody[] childrenRb; // 子オブジェクトのRigidbodyを格納

	private DestructibleObject[] destructibleObjects;


	BoxCollider boxCollider;


    // Start is called before the first frame update
    void Start()
    {
		parent = gameObject.transform;
		boxCollider = GetComponent<BoxCollider>();
		
		var children = new Transform[transform.childCount];
		childrenRb = new Rigidbody[transform.childCount];
		
		destructibleObjects = new DestructibleObject[transform.childCount];

		for (int i = 0; i < transform.childCount; i++)
		{
			children[i] = transform.GetChild(i);
			childrenRb[i] = children[i].GetComponent<Rigidbody>();
			destructibleObjects[i] = children[i].GetComponent<DestructibleObject>();
		}
	}
	
    // Update is called once per frame
    void Update()
    {
		var children = new Transform[transform.childCount];

		for (int i = 0; i < transform.childCount; i++)
		{
			children[i] = transform.GetChild(i);
			childrenRb[i] = children[i].GetComponent<Rigidbody>();
			destructibleObjects[i] = children[i].GetComponent<DestructibleObject>();
			if (destructibleObjects[i].IsDestroyed == true)
			{
				foreach (Rigidbody rb in childrenRb)
				{
					if (rb == null)
					{
						return;
					}

					if (rb.isKinematic == true)
					{
						rb.isKinematic = false;
					}
					MeshCollider meshCollider = rb.GetComponent<MeshCollider>();
					meshCollider.excludeLayers = LayerMask.GetMask("PlayerLayer");

					rb.constraints = RigidbodyConstraints.None;
				}
				boxCollider.enabled = false;
			}
		}

    }


	private void OnCollisionEnter(Collision collision)
	{
		// プレイヤーのSphereColliderに当たったら、
		var children = new Transform[transform.childCount];
		for (int i = 0; i < transform.childCount; i++)
		{

			if (destructibleObjects[i].IsDestroyed == true)
			{
				boxCollider.enabled = false;
			}

		}

	}
}
