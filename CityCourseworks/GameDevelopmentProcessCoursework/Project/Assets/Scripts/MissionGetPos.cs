using UnityEngine;
using System.Collections;
using Unity.VisualScripting;

public class MissionGetPos : MonoBehaviour
{
    public bool isCollisionMissionGetPos = false;
    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Player")
        {
            isCollisionMissionGetPos = true;
            //print(isCollisionMissionGetPos);
            //countdowntimer = 90;
            //print("MissionGetPos Object Collided");
            print("Order Collected");
            //Destroy(other.gameObject);

        }
    }

}
