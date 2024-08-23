using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEditor;
using UnityEngine.InputSystem;


public class ButtonHeldTrigger : MonoBehaviour
{
    Matrix4x4 thisTriggerMatrix;
    public InputAction UseButton;
    public string thisTriggerName;
    public string targetTag;
    public Color triggerColour = Color.green;
    Color triggerFillColour;
    public float downTime, upTime, pressTime = 0;
    public float countDown = 2.0f;
    public bool ready = false;
    public UnityEvent OnButtonHeldEvent;
    public UnityEvent OnTriggerEnterEvent;
    public UnityEvent OnTriggerStayEvent;
    public UnityEvent OnTriggerExitEvent;



    void OnEnable()
    {

        this.gameObject.layer = 2;
        this.GetComponent<Collider>().isTrigger = true;
        if (this.GetComponent<MeshRenderer>() == true)
        {
            DestroyImmediate(this.GetComponent<MeshRenderer>());
        }


    }

    void OnAwake()
    {
        UseButton.Enable();
    }

    void OnValidate()
    {
        triggerFillColour = new Color(triggerColour.r, triggerColour.g, triggerColour.b, 0.25f);
        gameObject.name = thisTriggerName + " (Button Held Trigger)";
      
    }

#if UNITY_EDITOR

    void OnDrawGizmos()
    {
        thisTriggerMatrix = this.transform.localToWorldMatrix;
        Gizmos.matrix = thisTriggerMatrix;
        Gizmos.color = triggerColour;
        Gizmos.DrawWireCube(GetComponent<BoxCollider>().center, GetComponent<BoxCollider>().size); //DrawCube does filled Cube
        Gizmos.color = triggerFillColour;
        Gizmos.DrawCube(GetComponent<BoxCollider>().center, GetComponent<BoxCollider>().size); //DrawCube does filled Cube
        GUIStyle handleStyle = new GUIStyle();
        handleStyle.alignment = TextAnchor.MiddleCenter;
        handleStyle.fontStyle = FontStyle.BoldAndItalic;
        handleStyle.normal.textColor = Color.white;
        Handles.Label(transform.position, gameObject.name, handleStyle);
    }
#endif

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == targetTag)
        {
            OnTriggerEnterEvent.Invoke();
        }
    }

    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.tag == targetTag)
        {
                OnTriggerStayEvent.Invoke();

            if (UseButton.IsPressed() && ready == false)
            {
                downTime = Time.time;
                pressTime = downTime + countDown;
                ready = true;
            }
            if (!UseButton.IsPressed())
            {
                ready = false;
            }
            if (Time.time >= pressTime && ready == true)
            {
                ready = false;
                OnButtonHeldEvent.Invoke();
            } 
               
            }
        
    }

  
    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.tag == targetTag)
        {
            OnTriggerExitEvent.Invoke();
        }

    }


}
