// “®•¨ƒ‰ƒ“ƒ_ƒ€¶¬

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class AnimalScript : MonoBehaviour
{
    public GameObject[] Animal;

    void Start()
    {
        int number = Random.Range(0, Animal.Length);
        Instantiate(Animal[number], transform.position, transform.rotation);
    }
}