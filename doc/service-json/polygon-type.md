# Polygon

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Polygon",
    "Data": {
        "Name": __name, 
        "Id": __id,
        "Width": __width,
        "Color": __color, // argb color string format example: "#aabbccdd"
        "FillColor": __fill_color,
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
    "Type": "Polygon",
    "Data": {
        "Name": "Node1 polygon",  
        "Id": 30001,
        "Width": 7.5,
        "Color": "#aabbccdd",
        "FillColor": "#aabbccdd",
        "Points": [
            {
                "Longitude": 11.1,
                "Latitude": 22.22,
                "Altitude": 333.333
            },
            {
                "Longitude": 4,
                "Latitude": 5,
                "Altitude": 6
            }
        ],
        "LayerId": 3
    }
}
```