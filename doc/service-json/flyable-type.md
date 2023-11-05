# Flyable

- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Flayable",
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
        "Heading": __heading, 
        "Speed": __speed, 
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
    "Type": "Flayable",
    "Data": {
        "Name": {
            "value": "Node1",
            "category": "Location Information"
        },
        "Id": {
            "value": 1,
            "category": "Location Information"
        },
        "Color": "#aabbcc",
        "Url2d": "../data/models/station/station.png",
        "Url3d": "../data/models/station/station.osgb",
        "ImgSrc": "qrc:/Resources/airplane1.jpg",
        "Longitude": {
            "value": 342.2,
            "category": "Location Information"
        },
        "Latitude": {
            "value": 123.1,
            "category": "Location Information"
        },
        "Altitude": {
            "value": 344.2,
            "category": "Location Information"
        },
        "Heading": {
            "value": 89.88,
            "category": "Location Information"
        },
        "Speed": {
            "value": 88.9,
            "category": "Location Information"
        },
        "LayersId": [
            1,
            2
        ]
    }
}
```

