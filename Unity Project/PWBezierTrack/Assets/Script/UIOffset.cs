using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.UI;
using System.Globalization;
using System;

public class UIOffset : MonoBehaviour
{

    [SerializeField] TMP_InputField U;
    [SerializeField] TMP_InputField V;
    [SerializeField] Button validateBtn;
    [SerializeField] Manager manager;
    [SerializeField] float animationTime = 5.0f;




    // Start is called before the first frame update
    void Start()
    {
        Debug.Assert(U != null);
        Debug.Assert(V != null);
        Debug.Assert(validateBtn != null);

        validateBtn.onClick.AddListener(() =>
        {
            try
            {
                var u = float.Parse(U.text, CultureInfo.InvariantCulture);
                var v = float.Parse(V.text, CultureInfo.InvariantCulture);

                Vector4 formerUV = manager.mat.GetVector("_Offset");

                var newUV = new Vector4(u, v, 0, 0);

                StartCoroutine(Interpolate(formerUV, newUV, animationTime));
            }
            catch (System.FormatException)
            {
                Debug.LogWarning("invalid number format");
            }



        });
    }


    IEnumerator Interpolate(Vector4 formerValue,Vector4 newValue, float duration)
    {
        float startTime = Time.time;
        float elapsedTime = Time.time - startTime;


        while (elapsedTime < duration)
        {
            elapsedTime = Time.time - startTime;
            // calculate the current value based on the elapsed time and duration
            float progress = Mathf.SmoothStep(0.0f, 1.0f, elapsedTime / duration);
            var currentVal = Vector4.Lerp(formerValue, newValue, progress);


            manager.mat.SetVector("_Offset", currentVal);
            // wait for a short time before calculating the next value
            yield return new WaitForSeconds(0.01f);



        }

        manager.mat.SetVector("_Offset", newValue);

    }




    // Update is called once per frame
    void Update()
    {
        
    }
}
