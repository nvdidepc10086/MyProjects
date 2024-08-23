using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.XR.Interaction.Toolkit.Inputs;

// Attach this script to the Direct Interactors under your left and right controllers under XR Origin. 
public class Shoot : XRGrabInteractable
{

    public XRBaseInteractable grabbedObject;
    // The Direct Interactor 
    public XRBaseInteractor thisInteractor;

    [SerializeField]
    public float BallShootForce;

 

    public void ShootBall()
    {
        // force to drop the ball from interactor
        grabbedObject = GetComponent<XRGrabInteractable>();
        if (grabbedObject != null && grabbedObject.isSelected) 
        {
            grabbedObject.enabled = false; 
        }

        grabbedObject.enabled = true;

        Rigidbody rb = grabbedObject.GetComponent<Rigidbody>();
        if (rb != null)
        {
            // Calculate shoot direction (in this example, it's the forward direction of the controller)
            Vector3 shootDirection = thisInteractor.transform.forward;

            // Apply force to shoot the object
            rb.AddForce(shootDirection * BallShootForce);
        }

    }

  
}


