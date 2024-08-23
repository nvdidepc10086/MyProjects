using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Score : MonoBehaviour
{
    private void OnCollisionEnter(Collision collision)
    {
        if(collision.gameObject.tag == "Score")
        {
            print("fucking score");
            scoreboard.Baskets += 1;
            Destroy(gameObject);
        }
    }

}
