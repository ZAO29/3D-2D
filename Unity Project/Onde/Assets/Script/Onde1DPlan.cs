using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using MF = UnityEngine.Mathf;

public class Onde1DPlan : MonoBehaviour
{
    [SerializeField] Transform Ext1;
    [SerializeField] Transform Ext2;
    [SerializeField] OndePlanShaderInterface ondeInterface;
    [SerializeField] LineRenderer lr;
    [SerializeField] uint resolution = 100;

    float arbitraryValue = 50.0f;

    // Start is called before the first frame update
    void Start()
    {
        lr = GetComponent<LineRenderer>();
        lr.positionCount = (int)resolution;

        Debug.Assert(Ext1 != null);
        Debug.Assert(Ext2 != null);
        Debug.Assert(ondeInterface != null);
        Debug.Assert(lr != null);

    }

    // Update is called once per frame
    void LateUpdate()
    {
        if(lr.positionCount != resolution)
        {
            lr.positionCount = (int) resolution;
        }

        for (int i = 0; i < lr.positionCount; i++)
        {
            var coeff = ((float)i) / (resolution - 1);
            var pos = Vector3.Lerp(Ext1.localPosition, Ext2.localPosition, coeff);

            Vector2 pos2D = pos;
            Vector3 rgbValue = Vector3.zero;
            for (int iSrc = 0; iSrc < ondeInterface.NbSource; iSrc++)
            {
                rgbValue += RGBondula(pos2D, ondeInterface.Sources(iSrc), ondeInterface.RGBShift);
            }

            Vector3 weights = new Vector3(1, 0.5f, 1.0f / 3.0f);
            float value = 0.0f;
            for (int iW=0;iW<3;iW++)
            {
                value += weights[iW] * rgbValue[iW];
            }

            value /= (weights[0] + weights[1] + weights[2]);

            lr.SetPosition(i, new Vector3(2.0f * Camera.main.aspect * (coeff-0.5f), 2.0f * value, 1));

             
        }


    }


    float ondula(Vector2 st, Vector2 center, float shift, float period)
    {
        float t = ondeInterface.t;
        float r = Vector2.Distance(st, center);
        float ondula = MF.Cos(r * period - 5.0f* t + shift) / 2.0f / (1.0f + 5.0f* r);
        return ondula;
    }


    Vector3 RGBondula(Vector2 st, Vector2 center, Vector3 RGBshift)
    {


        float period = ComputePeriod();
        float ondeR = ondula(st, center, RGBshift.x, period);
        float ondeG = ondula(st, center, RGBshift.y, 2.0f* period);
        float ondeB = ondula(st, center, RGBshift.z, 3.0f* period);

        return new Vector3(ondeR, ondeG, ondeB);

    }

    float ComputePeriod()
    {
        float val = arbitraryValue;
        float period = val + (1.0f + MF.Cos(ondeInterface.t)) / 2.0f * val;

        return period;
    }

}
