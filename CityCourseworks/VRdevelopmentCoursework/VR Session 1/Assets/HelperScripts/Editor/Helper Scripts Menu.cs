using UnityEditor;
using UnityEngine;
using UnityEngine.Events;
public class HelpScriptsMenu : MonoBehaviour
{

    [MenuItem("GameObject/Helper Scripts/Enter Trigger")]
    static void CreateTrigger(MenuCommand command)
    {
        GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
        go.AddComponent<EnterTrigger>();
        Selection.activeGameObject = go.gameObject;

    }

    [MenuItem("GameObject/Helper Scripts/Button Pressed Trigger")]
    static void CreateButtonPressedTrigger(MenuCommand command)
    {
        GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
        go.AddComponent<ButtonPressedTrigger>();
        Selection.activeGameObject = go.gameObject;

    }

    [MenuItem("GameObject/Helper Scripts/Button Toggle Trigger")]
    static void CreateButtonToggleTrigger(MenuCommand command)
    {
        GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
        go.AddComponent<ButtonToggleTrigger>();
        Selection.activeGameObject = go.gameObject;

    }

    [MenuItem("GameObject/Helper Scripts/Button Held Trigger")]
    static void CreateButtonHeldTrigger(MenuCommand command)
    {
        GameObject go = GameObject.CreatePrimitive(PrimitiveType.Cube);
        go.AddComponent<ButtonHeldTrigger>();
        Selection.activeGameObject = go.gameObject;
    }


    [MenuItem("GameObject/Helper Scripts/Spatial Sound Source")]
    static void CreateSpatialSoundSource(MenuCommand command)
    {
        GameObject go = new GameObject("SpatialSoundSource");
        go.AddComponent<SpatialSoundSource>();
        Selection.activeGameObject = go.gameObject;
    }
}
