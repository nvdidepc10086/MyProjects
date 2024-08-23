using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BounceSound : MonoBehaviour
{
    public AudioSource BounceSource;


    public void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "BounceSound")
        {
            print("fucking bouncing");
            BounceSource.Play();

        }
    }
}
