# Flyable Node Type

- [Back](./README.md)
- [Example](#example)
- [See basic needed types here](#json-basic-types)

```JSON
{
    "Type": "Flayable",
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
        "Heading": __heading, // <CategorizedPropertyType> 
        "Speed": __speed, // <CategorizedPropertyType> 
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

