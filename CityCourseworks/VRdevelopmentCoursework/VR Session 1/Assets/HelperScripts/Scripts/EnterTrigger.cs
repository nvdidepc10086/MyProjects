using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using UnityEditor;

[ExecuteInEditMode]
public class EnterTrigger : MonoBehaviour
{
    Matrix4x4 thisTriggerMatrix;
    public string thisTriggerName;
    public string targetTag;
    public Color triggerColour = new Color(143, 0, 254, 1f);
    public float size;
    Color triggerFillColour;
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

    void OnValidate()
    {
       triggerFillColour = new Color(triggerColour.r, triggerColour.g, triggerColour.b, 0.25f);
       gameObject.name = thisTriggerName + " (Trigger)";   
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
