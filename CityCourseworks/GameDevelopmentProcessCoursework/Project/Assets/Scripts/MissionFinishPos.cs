using UnityEngine;
using System.Collections;
using Unity.VisualScripting;
using System.Diagnostics;

public class MissionFinishPos : MonoBehaviour
{
    public bool isCollisionMissionFinishPos = false;
    private void OnTriggerEnter(Collider other)
    {
        if (GameObject.FindObjectOfType<MissionGetPos>().isCollisionMissionGetPos)
        {
            if (other.gameObject.tag == "Player")
            {
                isCollisionMissionFinishPos = true;
                print("Order Finished");
                //Destroy(this.gameObject);
            }
        }
    }
}
