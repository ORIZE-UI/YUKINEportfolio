using System.Collections.Generic;
using UnityEngine;

public class Attractor : MonoBehaviour
{
	public float attractRadius = 5.0f; // 引力を持つ範囲の半径
	public float attractionStrength = 10.0f; // 引力の強さ
	public float destroyRadius = 1.0f; // デストロイする範囲の半径
	private List<GameObject> attractedObjects = new List<GameObject>();

	void Update()
	{
		Collider[] colliders = Physics.OverlapSphere(transform.position, attractRadius);
		attractedObjects.Clear();

		foreach (Collider collider in colliders)
		{
			if (collider.CompareTag("Enemy"))
			{
				attractedObjects.Add(collider.gameObject);
			}
		}

		foreach (GameObject obj in attractedObjects)
		{
			Attract(obj);
		}
	}

	void Attract(GameObject obj)
	{
		Vector3 direction = transform.position - obj.transform.position;
		float distance = direction.magnitude;

		// デストロイする範囲内に入ったらEnemyタグのオブジェクトをデストロイ
		if (distance <= destroyRadius && obj.CompareTag("Enemy"))
		{
			Destroy(obj);
		}
		else
		{
			float forceMagnitude = (attractionStrength / distance) * Time.deltaTime;
			Vector3 force = direction.normalized * forceMagnitude;
			Rigidbody rb = obj.GetComponent<Rigidbody>();

			if (rb != null)
			{
				rb.AddForce(force);
			}
		}
	}

	void OnDrawGizmosSelected()
	{
		Gizmos.color = Color.yellow;
		Gizmos.DrawWireSphere(transform.position, attractRadius);

		Gizmos.color = Color.red;
		Gizmos.DrawWireSphere(transform.position, destroyRadius);
	}
}
