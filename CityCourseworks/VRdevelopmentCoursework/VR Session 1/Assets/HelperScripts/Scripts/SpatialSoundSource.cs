using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[ExecuteInEditMode]
[RequireComponent(typeof(AudioSource))]
public class SpatialSoundSource : MonoBehaviour
{
    private AudioSource thisSource;
    public AudioClip thisSound;
    public string thisSoundName;
    public float SoundMinDistance;
    public float SoundMaxDistance;
    [Range(0.0F, 1.0f)]
    public float SoundVolume;
    public bool Looping = true;
    public bool PlayOnAwake = true;
    
    // Start is called before the first frame update
    void Awake()
    {
        thisSource = GetComponent<AudioSource>();
        thisSource.clip = thisSound;
        thisSource.volume = SoundVolume;
        thisSource.spatialBlend = 1.0f;
        thisSource.minDistance = SoundMinDistance;
        thisSource.maxDistance = SoundMaxDistance;
        thisSource.rolloffMode = AudioRolloffMode.Linear;
        thisSource.loop = Looping;
        thisSource.playOnAwake = PlayOnAwake;
    }

    private void OnValidate()
    {
        if (gameObject.name == null)
        {
            gameObject.name = thisSoundName + " (Spatial Sound Source)";
        }
     }

        // Update is called once per frame
        void Update()
    {
        if(thisSource.clip != thisSound)
        {
            thisSource.clip = thisSound;
        }

        if (thisSource.volume != SoundVolume)
        {
            thisSource.volume = SoundVolume;
        }

        if (thisSource.minDistance != SoundMinDistance)
        {
            thisSource.minDistance = SoundMinDistance;
        }

        if (thisSource.maxDistance != SoundMaxDistance)
        {
            thisSource.maxDistance = SoundMaxDistance;
        }

    }

    void SetUpSpatialSound()
    {

    }
}
