using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// Add this script to a gameobject with a trigger you want to use as a health pick-up. 
public class HealthPickUp : MonoBehaviour
{
	// Runs when a gameobject enters this trigger area. 
    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.GetComponent<Health>() == true) // Checks if it has a health script attached. 
        {
            if (other.gameObject.GetComponent<Health>().GetHealth() < 100) // If health is under 100
            {
                other.gameObject.GetComponent<Health>().IncreaseHealth(15.0f); // Increase health by set amount.
                Destroy(this.gameObject); // Destroy the health pickup. 
            }
        }
    }
 
}
