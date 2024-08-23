using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LinearRotator : MonoBehaviour
{
    /// <summary>
    /// The duration of a full day/night cycle
    /// in seconds
    /// </summary>
    public float m_fullRotationDuration;

    /// <summary>
    /// Called when placing a script through the inspector
    /// </summary>
    void Reset()
    {
        m_fullRotationDuration = 90f;
    }

    /// <summary>
    /// Update is called internally, once every frame
    /// </summary>
    void Update()
    {
        //amount of degrees that the object should rotate;
        //depends on the time that passed since the last Update call.
        float degrees = (360f / m_fullRotationDuration) * Time.deltaTime;

        //access the Transform component of 'this' game object
        //and rotate it along the X axis
        transform.Rotate(-Vector3.right, degrees);
    }
}
