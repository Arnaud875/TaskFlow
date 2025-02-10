import React, { useState } from "react";
import { ConfigProvider, Typography, Button, Modal } from "antd";
const { Title } = Typography;

export default function App() {
  const [isModalOpen, setIsModalOpen] = useState(false);
  const [isModalLoading, setIsModalLoading] = useState(false);
  const [message, setMessage] = useState("Waiting api...");

  const showLoading = () => {
    setIsModalOpen(true);
    setIsModalLoading(true);

    fetch("http://localhost:8080/api/test")
      .then(async (response) => {
        if (response.status === 200) {
          const data = await response.json();
          setMessage(data["message"]);
        } else {
          setMessage("API call failed!");
        }
        setIsModalLoading(false);
      });
  }

  return (
    <ConfigProvider>
      <div style={{
        height: '100vh',
        display: 'flex',
        flexDirection: 'column',
        justifyContent: 'center',
        alignItems: 'center',
        gap: '20px'
      }}>
        <Title level={1}>Hello, World !</Title>
        <Button type="primary" size="large" onClick={showLoading}>
          Click Me!
        </Button>
      </div>

      <Modal title="Hello, World !" loading={isModalLoading} open={isModalOpen} onOk={() => setIsModalOpen(false)} onCancel={() => setIsModalOpen(false)}>
        <p>{message}</p>
      </Modal>
    </ConfigProvider>
  )
}
