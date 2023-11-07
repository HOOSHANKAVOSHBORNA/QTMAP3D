# Node

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Node",
    "Data": { // all `values` can be a simple type or <CategorizedPropertyType> object
        "Name": __name, 
        "Id": __id, 
        "Color": __color,
        "Url2d": __url2d,
        "Url3d": __url3d,
        "ImgSrc": __image_source,
        "Longitude": __longitude, 
        "Latitude": __latitude, 
        "Altitude": __altitude,
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