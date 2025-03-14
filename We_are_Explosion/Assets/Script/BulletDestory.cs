using UnityEngine;

public class DestroyObject : MonoBehaviour
{
	
	public float lifeTime = 5f;

	
	void Start()
	{
		
		Destroy(gameObject, lifeTime);
	}


	private void OnTriggerEnter(Collider other)
	{
		if (other.gameObject.CompareTag("Wall"))
		{
			Destroy(gameObject);
		}
	}

}
