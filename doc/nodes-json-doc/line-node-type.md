# Line Node Type

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Line",
    "Data": {
        "Name": __name,
        "Id": __id,
        "Color": __color,
        "Width": __width,
        "Points": [
            __point1, // <PointType>
            __point2, // <PointType>
            ...
        ],
        "LayerId": __layer_id
    }
}
```

## JSON Basic Types 

```JSON
<PointType> = {
    "Longitude": __longitude,
    "Latitude": __latitude,
    "Altitude": __altitude
}
```

## Example

```JSON
{
    "Type": "Line",
    "Data": {
        "Name": "Flyable2",
        "Id": 22,
        "Color": "#aabbcc",
        "Width": 22.3,
        "Points": [
            {
                "Longitude": 11.22,
                "Latitude": 22.33,
                "Altitude": 33.44
            },
            {
                "Longitude": 33.44,
                "Latitude": 44.55,
                "Altitude": 55.66
            }
        ],
        "LayerId": 2
    }
}
```

