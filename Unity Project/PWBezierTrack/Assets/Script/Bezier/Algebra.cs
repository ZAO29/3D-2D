using System.Collections;
using System.Collections.Generic;
using UnityEngine;


// un example d'extension
public static class AlgebraExtension 
{



    public static Vector2 Rotate(this Vector2 v, float degree)
    {
        var radians = degree * Mathf.Deg2Rad;
        return new Vector2(
            v.x * Mathf.Cos(radians) - v.y * Mathf.Sin(radians),
            v.x * Mathf.Sin(radians) + v.y * Mathf.Cos(radians)
        );
    }


    public static int IdOne(this Vector2 v)
    {
        if(v == Vector2.right)
            return 0;

        if(v == Vector2.up)
            return 1;

        return -1;
    }



    public static Vector3 Vec3Z0(this Vector2 vector2)
    {
        return new Vector3(vector2.x, vector2.y, 0.0f);
    }

    
}
