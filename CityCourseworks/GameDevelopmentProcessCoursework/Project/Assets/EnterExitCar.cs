using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnterExitCar : MonoBehaviour
{
    public Transform player;

    public bool carActive;
    bool isInTransition;
    public Transform seatPoint;
    public Transform exitPoint;
    public Vector3 sittingOffset;

    public float transitionSpeed = 0.2f;

    private void Update()
    {
        if (carActive && isInTransition)
        {
            Exit();
        }
        else if (!carActive && isInTransition)
        {
            Enter();
        }

        if (Input.GetKeyDown(KeyCode.E))
        {
            isInTransition = true;
        }
    }

    private void Enter()
    {
        //disable components
        player.GetComponent<CapsuleCollider>().enabled = false;
        player.GetComponent<Rigidbody>().useGravity = false;

        //move player to seat
        player.position = Vector3.Lerp(player.position, seatPoint.position + sittingOffset, transitionSpeed);
        player.rotation = Quaternion.Slerp(player.rotation, seatPoint.rotation, transitionSpeed);

        //set player animation to sitting
        player.GetComponent<Animator>().SetBool("isSitting", true);

        if (player.position == seatPoint.position + sittingOffset)
        {
            isInTransition = false;
            carActive = true;
        }
    }

    private void Exit()
    {
        // move player to exit point
        player.position = Vector3.Lerp(player.position, exitPoint.position, transitionSpeed);

        // set player animation to happy idle
        player.GetComponent<Animator>().SetBool("isSitting", false);

        if (player.position == exitPoint.position)
        {
            isInTransition = false;
            carActive = false;
        }

        // enable components
        player.GetComponent<CapsuleCollider>().enabled = true;
        player.GetComponent<Rigidbody>().useGravity = true;

    }
}
