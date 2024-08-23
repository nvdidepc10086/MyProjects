
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.XR.Interaction.Toolkit;
using UnityEngine.XR.Interaction.Toolkit.Inputs;

// Attach this script to the Direct Interactors under your left and right controllers under XR Origin. 
public class Throw : Shoot
{

    // The Direct Interactor 
    public XRBaseInteractor thisInteractor;

    public InputAction shootAction;

    //ActionBasedController controller;




    public void BallThrow()
    {
        // force to drop the ball from interactor


        
        if (grabbedObject.GetComponent<Rigidbody>() != null)
        {
            // Calculate shoot direction (in this example, it's the forward direction of the controller)
            Vector3 shootDirection = thisInteractor.transform.forward;


            // Apply force to shoot the object
            grabbedObject.GetComponent<Rigidbody>().AddForce(shootDirection * BallShootForce);

            // set enable to make ball can be grabbed
            grabbedObject.enabled = true;
        }
        



    }


}


