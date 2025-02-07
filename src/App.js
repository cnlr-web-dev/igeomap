import { useLocation } from "react-router";
import 'leaflet/dist/leaflet.css';
import { MapContainer, TileLayer, FeatureGroup, Rectangle, LayersControl, ImageOverlay } from 'react-leaflet';
import MapNavbar from "./components/mapnavbar";
import MapDrawer from "./components/mapdrawer";
import GranulePopup from "./components/mapgranulepopup";
import granules from "./data/granules";

function App() {
  const clocation = useLocation();
  var center = [46, 24] // Romania

  if (localStorage.lastGranuleCoord) {
    center = [
      (JSON.parse(localStorage.lastGranuleCoord)[0][0] + JSON.parse(localStorage.lastGranuleCoord)[1][0]) / 2,
      (JSON.parse(localStorage.lastGranuleCoord)[0][1] + JSON.parse(localStorage.lastGranuleCoord)[1][1]) / 2
    ];
  }

  return (
    <div className="App">
      <MapDrawer location={clocation}>
        <MapNavbar />

        <MapContainer style={{ zIndex: 0 }} className="markercluster-map" center={center} zoom={localStorage.lastGranuleCoord ? 8 : 7} scrollWheelZoom={true} minZoom={7} maxBounds={[[52, 10] /* Centrul Germaniei */, [40, 38.5] /* Estul Turciei */]}>
          <TileLayer
            attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
            url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
          />
          <LayersControl position="topright">

            <LayersControl.Overlay checked={true} name="Date hgt preluate din NASA Shuttle Radar Topography Mission Global 1 arc second V003">

              <FeatureGroup pathOptions={{ color: 'rgba(128, 0, 128, 0.1)' }}>
                {granules.map((granule, index) => (
                  <div key={index}>
                    <Rectangle bounds={granule.coord}>
                      <GranulePopup granule={granule} />
                    </Rectangle>

                    <ImageOverlay
                      url={granule.image}
                      bounds={granule.coord}
                      opacity={0.7}
                      zIndex={0}
                    />
                  </div>

                ))}
              </FeatureGroup>

            </LayersControl.Overlay>
          </LayersControl>
        </MapContainer>
      </MapDrawer>
    </div>
  );
}

export default App;