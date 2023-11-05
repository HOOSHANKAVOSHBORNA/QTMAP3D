# Circle

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Circle",
    "Data": {
        "Name": __name,  
        "Id": __id,
        "Url2d": __url2d,
        "Url3d": __url3d,
        "ImgSrc": __image_source,
        "Longitude": __longitude, 
        "Latitude": __latitude, 
        "Altitude": __altitude,
        "Radius": __radius,
        "Color": __color, // argb format string example: "#12345678"
        "LayerId": __layer_id
    }
}
```

## JSON Basic Types 

**No Basic Needed!**

## Example

```JSON
{
    "Type": "Circle",
    "Data": {
        "Name": "example circle",  
        "Id": 1,
        "Url2d": "../data/models/station/station.png",
        "Url3d": "../data/models/station/station.osgb",
        "ImgSrc": "qrc:/Resources/system.jpg",
        "Longitude": 2.3, 
        "Latitude": 4.5, 
        "Altitude": 3.4,
        "Radius": 22.3,
        "Color": "#aabbccdd",
        "LayerId": 1
    }
}
```