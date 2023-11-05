# Node Node Type

- [Back](./README.md)
- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Node",
    "Data": {
        "Name": __name, // <CategorizedPropertyType> 
        "Id": __id, // <CategorizedPropertyType> 
        "Color": __color,
        "Url2d": __url2d,
        "Url3d": __url3d,
        "ImgSrc": __image_source,
        "Longitude": __longitude, // <CategorizedPropertyType> 
        "Latitude": __latitude, // <CategorizedPropertyType> 
        "Altitude": __altitude, // <CategorizedPropertyType>
        "LayersId": [
            __layer1id,
            __layer2id,
            ...
        ]
    }
}
```

## JSON Basic Types 

```JSON
<CategorizedPropertyType> = {
    "value": __value,
    "category": __category
}
```

## Example

```JSON
{
    "Type": "Node",
    "Data": {
        "Name": {
            "value": "Node2",
            "category": "Main Information"
        }, 
        "Id": {
            "value": 30001,
            "category": "Main Information"
        }, 
        "Color": "#aabbcc",
        "Url2d": "../data/models/station/station.png",
        "Url3d": "../data/models/station/station.osgb",
        "ImgSrc": "qrc:/Resources/system.jpg",
        "Longitude": {
            "value": 33.33,
            "category": "Location Information"
        }, 
        "Latitude": {
            "value": 33.33,
            "category": "Location Information"
        }, 
        "Altitude": {
            "value": 33.33,
            "category": "Location Information"
        },
        "LayersId": [
            200,
            233
        ]
    }
}
```