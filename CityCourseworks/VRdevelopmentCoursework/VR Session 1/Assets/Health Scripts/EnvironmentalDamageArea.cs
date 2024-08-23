using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnvironmentalDamageArea : MonoBehaviour
{
    private GameObject targetObject;
    private bool inArea = false;
    public string targetTag = "Player"; // Tag for GameObject you want the environmental effect to damage. 

	// Function runs when a gameobject enters this trigger area. 
    private void OnTriggerEnter(Collider other)
    {
		// Checks if it has a Health script attached and its tag matches the target tag.
        if (other.gameObject.GetComponent<Health>() == true && other.gameObject.tag == targetTag)
        {
            targetObject = other.gameObject; // Sets target object as the object that entered this area.
            inArea = true; // Bool to track if the target is in the area.
            StartCoroutine(DepleteHealth(2.0f)); // Coroutine to reduce health after a given wait time

        }
    }
	
	// Function runs when a gameobject leaves this trigger area. 
    private void OnTriggerExit(Collider other)
    {
		// Checks if it has a Health script attached and its tag matches the target tag.
        if (other.gameObject.GetComponent<Health>() == true && other.gameObject.tag == targetTag)
        {
            StopCoroutine("DepleteHealth"); // Stops the decrease health coroutine.
            inArea = false; // Sets bool to false. 
        }
    }

    private IEnumerator DepleteHealth(float waitTime)
    {
		// While the target is in the trigger area. 
        while (inArea == true)
        {
            yield return new WaitForSeconds(waitTime); // Wait for the time specified.
            targetObject.GetComponent<Health>().Damage("Environment", 2.0f); // Apply environmental damage to the target by set amount.
            targetObject.GetComponent<Health>().InCombat(true); // Set that the target is in combat.
        }
 
    }
}
